# Denuvo Analysis — Combined Reference

Two sources merged: Connor Jay Dunn's educational blog post on Denuvo internals
(2025-01-21) and 0xPacman's DenuvOwO hypervisor bypass RE report (2026-05-13).

---

# Part I — Denuvo Internals

*Source: connorjaydunn.github.io (2025-01-21)*

## The General Idea

Denuvo is a semi-online DRM. The flow:

1. User boots `program.exe` for the first time.
2. Before any game code runs, Denuvo collects hardware identification.
3. The game sends this hardware info to a Denuvo server. The server combines "stolen constants" with the hardware info via reversible math and returns a "license file."
4. A local copy of the license file is cached (semi-online — no repeated network calls).
5. The game is redirected to the OEP. At runtime, Denuvo collects hardware info again and attempts to decrypt stolen constants from the license file. If the hardware matches, the correct constants are decrypted and "original game instructions" execute.

If hardware info at runtime doesn't match what was used to create the license file, an incorrect constant is decrypted → crash.

## License File and Stolen Constants

When Denuvo is added to a binary, certain functions are selected for "protection" — they're executed inside a virtual machine with select instruction parts removed entirely from the binary.

The **license file** is all removed bytes combined with the user's hardware identification via reversible mathematical functions.

**License DWORDs**: Denuvo writes select parts of the license file into DWORDs scattered around the `.vm` section. Each DWORD is a single removed instruction combined with hardware identification.

### Example: How Instructions Are Removed

```asm
add(int, int):
    push  rbp
    mov  rbp, rsp
    mov  DWORD PTR [rbp-4], edi    ; Denuvo strips the constant -4
    mov  DWORD PTR [rbp-8], esi
    mov  edx, DWORD PTR [rbp-4]
    mov  eax, DWORD PTR [rbp-8]
    add  eax, edx
    pop  rbp
    ret
```

The constant `-4` is stripped and stored on Denuvo's server. The VM wrapper:
1. Collects hardware information at runtime
2. Reads the corresponding license DWORD
3. Performs mathematical operations to decrypt the constant
4. Executes the original instruction with the decrypted constant

## User Integrity Checks

### Pre-OEP Checks

Before handing control to the OEP, Denuvo performs constant decryptions and verifies they equal expected values. If not, the locally saved license file is deleted and a new one is requested from the server.

### KUSER_SHARED_DATA

A read-only page (4096 bytes) mapped into every Windows process. Denuvo uses:

| Offset | Field |
|--------|-------|
| 0x026C | NtMajorVersion |
| 0x02E8 | NumberOfPhysicalPages |
| 0x02D0 | SuiteMask |
| 0x0260 | NtBuildNumber |
| 0x0264 | NtProductType |
| 0x0268 | ProductTypeIsValid |
| 0x0270 | NtMinorVersion |
| 0x0274 | ProcessorFeatures[0x40] |
| 0x026A | NativeProcessorArchitecture |
| 0x03C0 | ActiveProcessorCount |

(64-bit offsets)

### CPUID

Denuvo uses:
- `EAX=0x1`: Processor Info and Feature Bits
- `EAX=0x80000001`: Extended Processor Info and Feature Bits
- `EAX=0x80000002, 0x80000003, 0x80000004`: Processor Brand String

### SYSCALL

Uses parameter `0x36` — `NtQuerySystemInformation`.

### NTDLL Checks

- **Function byte checks**: Denuvo identifies the user based on bytes of certain ntdll.dll functions and their RVA.
- **Image Data Directory**: Export/Import/Resource/Exception/Relocation Directory RVA and Size fields — ntdll.dll changes per Windows update, making it an ideal fingerprint.

### PEB (Process Environment Block)

| Offset | Field |
|--------|-------|
| 0x0118 | OSMajorVersion |
| 0x011C | OSMinorVersion |
| 0x012C | ImageSubsystemMajorVersion |
| 0x0130 | ImageSubsystemMinorVersion |

### Other Checks

- **XGETBV**: Reads extended control registers for CPU specifics.
- **GetWindowsDirectoryW**, **GetVolumeInformationW**, **GetComputerNameW**, **GetUsernameW**: Filesystem and identity fingerprinting.

## Code Integrity Checks

### CRC Checks

- **VM Handler CRC**: Scans important handlers (CPUID, SYSCALL, etc.) for hooks/tampering.
- **Seemingly Random .VM Check**: Reads a "random" amount of bytes from `.vm` to construct constants used in calculations that break if the bytes change. Example:

```nasm
movsx r13, word ptr ds:[0x00000001467FEE8D]  ; "random" word from .VM
add r13, 0xFFFFFFFFFFFFDBAB                     ; decrypt
add rax, r13                                     ; update VM instruction pointer
```

If a breakpoint/hook tampered with that word, the VM executes random handlers → undefined behavior.

## Virtual Machine

Denuvo doesn't store values in contiguous memory. Register values have their bytes/bits scattered everywhere ("bit vector"). This makes it extremely difficult to track operations on values.

### Randomness

Denuvo doesn't use API calls or `RDRAND`. Instead, it generates randomness from native register values using modular arithmetic:

```c
if (VCTX[0] % 9 == 0) {  // VCTX -> VM Context
    CPUID_A();
} else {
    CPUID_B();  // semantically identical to CPUID_A
}
```

### Mixed-Boolean-Arithmetic (MBA)

Denuvo exploits MBA to rewrite x86 instructions as opaque expressions:

- `x + y = (x & y) + (x | y)`
- `x | y = x + y + 1 + (~x | ~y)`
- `x - y = (x ^ -y) + 2*(x & -y)`

By zhou2007 (Theorem 2), every bitwise expression has a non-trivial linear MBA expression. Denuvo applies this recursively, producing deeply obfuscated VM code.

### On-The-Fly Decrypted+Re-Encrypted CPUID

Denuvo decrypts a CPUID in the VM section, executes it, then re-encrypts it. Since VM handlers are shared across threads, a spin-lock prevents concurrent execution of the same encrypted CPUID. Denuvo encrypts the spin-lock too, which requires yet another spin-lock.

### Anti-Exception-Based Hooking

Early attacks replaced CPUID/SYSCALL with `UD2` to trigger `INVALID_OPCODE_EXCEPTION`, then hooked `KiUserExceptionDispatcher`. Denuvo's counter: before executing the CPUID handler, it writes important values high up in "unused" stack space. Later, it retrieves those values for critical calculations. When an exception fires, Windows writes an `EXCEPTION_RECORD` into that same stack space → overwrites the values → undefined behavior.

## Cracking Approaches

1. **Patching Hardware ID Checks**: Extremely difficult due to CRC + randomness.
2. **Patching Constant Decryption**: More reasonable (no CRC/randomness on these routines), but finding a single decryption in 10M+ instructions is non-trivial.
3. **Complete Binary Restoration**: Requires fix-up/devirt of thousands of instructions. One instance of complete restoration is known.
4. **Hypervisor**: Spoof all hardware information at the hypervisor level. AMD SVM and Intel VT-x both support CPUID/XGETBV interception and SYSCALL hooking.

---

# Part II — DenuvOwO Hypervisor Bypass

*Source: 0xPacman (2026-05-13)*

## Executive Summary

**DenuvOwO** is a Denuvo anti-tamper bypass built on two open-source hypervisor research projects:
- **AMD path:** SimpleSvm (AMD SVM/AMD-V)
- **Intel path:** HyperDbg (Intel VT-x/EPT)

The crack loads an unsigned kernel driver at ring -1, placing the entire OS — including Denuvo — inside a VM it controls. From this position it intercepts and spoofs every check Denuvo performs.

## File Inventory

| File | Size | Role |
|------|------|------|
| `hypervisor-launcher.exe` | 334 KB | Usermode orchestrator (C++/MSVC) |
| `VBS.cmd` | 50 KB | OS security stripper |
| `driver_amd/SimpleSvm.sys` | 18 KB | AMD SVM hypervisor |
| `driver_intel/hyperkd.sys` | 12 KB | Intel HV loader (thin) |
| `driver_intel/hyperhv.dll` | 538 KB | Intel VT-x VMM core (~296 exports) |
| `driver_intel/hyperevade.dll` | 7 KB | Evasion/transparency callbacks |
| `driver_intel/hyperlog.dll` | 14 KB | Logging subsystem |
| `DenuoOwO_SRC.7z` | 7.6 MB | Full driver source code |

## Execution Flow

```
[Boot with DSE disabled via F7]
        │
        ▼
[hypervisor-launcher.exe]
  ├─ Acquires SE_DEBUG + SE_SYSTEM_ENVIRONMENT privilege
  ├─ Finds ntoskrnl.exe + CI.dll base in kernel
  ├─ Detects CPU: "GenuineIntel" → Intel / "AuthenticAMD" → AMD
  ├─ Copies unsigned .sys driver to %TEMP%\
  ├─ Creates kernel service via SCM → StartService → driver loads
  └─ Steals Explorer token → launches game as non-admin
        │
        ▼
[Driver loads at ring -1]
  AMD: SimpleSvm.sys → VMRUN on all cores
  Intel: hyperkd.sys → VMXON + VMLAUNCH on all cores
        │
        ▼
[OS + Denuvo + game run as guest VM]
        │
  CPUID leaf 1     → clears ECX bit 31 (Hypervisor Present bit)
  CPUID 0x40000000 → zeros all output registers (no vendor string)
  RDTSC timing     → CounterUpdater spoofs TSC delta
  SYSCALL          → callback table intercepts + patches return values
  Read own code    → EPT serves original unmodified page
  Execute own code → EPT maps to patched shadow page
  RFLAGS.TF        → routes #DB to original handler
        │
        ▼
[Game exits → driver unloaded → system returns to normal]
```

## VBS.cmd — OS Security Demolition

Disables before each gaming session (tracked in `HKLM\SOFTWARE\ManageVBS` for reversal):

| Feature | Why It Must Go |
|---------|---------------|
| VBS | Windows runs its own hypervisor at ring -1; two HVs can't coexist |
| HVCI | Would reject unsigned kernel driver code pages |
| Credential Guard | Runs inside VBS |
| System Guard | HW root-of-trust attestation would detect HV manipulation |
| KVA Shadow | Conflicts with crack's syscall hook on older Intel CPUs |
| DSE (Driver Signature Enforcement) | F7 at boot (one-time) allows unsigned .sys |
| BitLocker | Prevents recovery key prompt at Startup Settings |

**Revert:** All changes tracked under `HKLM\SOFTWARE\ManageVBS`. Running option 3 re-enables exactly what was disabled.

## AMD Path — SimpleSvm.sys

Lean SVM hypervisor (~9.9 KB code). DriverEntry calls exactly two functions: stack cookie init and SVM initialization.

### SVM Initialization

1. Raises IRQL to DISPATCH_LEVEL
2. Gets logical CPU count
3. Allocates VMCB (VM Control Block) per core — must be physically contiguous
4. Converts VMCB virtual → physical (`MmGetPhysicalAddress`)
5. On each core: executes `VMRUN` (AMD SVM root entry)
6. Registers power/driver-load callbacks
7. Watches for game process via `PsSetCreateProcessNotifyRoutine`

### Key Concepts

- **VMCB** defines guest state: all registers, segment descriptors, MSRs
- **Host saves state to VMSA** (VM Save Area)
- `VMRUN` switches to guest; `#VMEXIT` returns to host handler
- VMCB contains intercept bitmaps for: exceptions, I/O, MSR reads/writes, CPUID, RDTSC

## Intel Path — HyperDbg Stack

### hyperkd.sys — Thin Loader

Pure bridge. Calls `VmFuncInitVmm` (VMXON on all cores), registers Unload routine that calls `VmFuncUninitVmm`.

### hyperhv.dll — VT-x VMM Core (538 KB, ~296 exports)

Based on HyperDbg (hyperdbg.com), a full open-source Intel VT-x hypervisor with EPT.

#### VMM Initialization

```
VmFuncInitVmm
  └─ VmxCheckVmxSupport (CPUID.1:ECX.5)
      └─ VmxPerformVirtualizationOnAllCores
            └─ [DPC to each core]
                 └─ VmxPerformVirtualizationOnSpecificCore
                      ├─ VmxAllocateVmxonRegion (4KB contiguous)
                      ├─ VmxAllocateVmcsRegion (4KB contiguous)
                      ├─ VmxAllocateHostGdt/Idt/Tss/Stack
                      ├─ VmxAllocateMsrBitmap/IoBitmaps
                      ├─ EptAllocateAndCreateIdentityPageTable
                      └─ VmxVirtualizeCurrentSystem
                           ├─ VMXON → VMCLEAR → VMPTRLD → VMWRITE → VMLAUNCH
                           └─ LstarHook (syscall handler)
```

#### VMEXIT Handler

| Exit Reason | Handler | Denuvo Relevance |
|-------------|---------|-----------------|
| CPUID | `TransparentCheckAndModifyCpuid` | Hides hypervisor from CPUID |
| RDMSR | `TransparentCheckAndModifyMsrRead` | Spoofs MSR values |
| WRMSR | `TransparentCheckAndModifyMsrWrite` | Monitors MSR writes |
| VMCALL | `VmxVmcallHandler` | Internal HV control channel |
| EPT violation | `EptHandleEptViolation` | Memory hook dispatch |
| RDTSC/RDTSCP | Counter spoofing | Anti-timing-attack |
| MOV to CR3 | CR3 exit handler | Process switch monitoring |
| EFER syscall | `LstarHook` path | Syscall intercept |

#### EPT Hook Subsystem

The core technique for invisible code modification:

```
Normal:    Guest virtual → Guest physical → real code
After hook: Execute → shadow page (patched) / Read → original page
Result: Denuvo reads its own code as unmodified, but executes patched version
```

#### LstarHook — Syscall Interception

LSTAR = `IA32_LSTAR MSR` (0xC0000082) = kernel entry point for SYSCALL = `KiSystemCall64`.

1. VMM reads current LSTAR (saves as `OrigLstar`)
2. Sets up EFER.SCE VMEXIT — every SYSCALL causes a VMEXIT
3. VMEXIT handler calls `TransparentHandleSystemCallHook`
4. Callback inspects/modifies syscall number and arguments
5. After syscall: `TransparentCallbackHandleAfterSyscall` can modify return values

This intercepts **every system call** made by Denuvo.

#### TSC Spoofing

Denuvo uses RDTSC/RDTSCP timing to detect VM overhead. The `CounterUpdater` thread maintains a fake TSC counter that advances at realistic speed, masking VMEXIT overhead.

## hyperevade.dll — Evasion Layer

7 KB, the most Denuvo-specific component. Provides callbacks called from VMEXIT handlers.

### TransparentCheckAndModifyCpuid

Two anti-detection behaviors:

1. **CPUID Leaf 1**: Clears ECX bit 31 — the "Hypervisor Present" bit. Denuvo checks this → sees bare metal.
2. **CPUID Leaves 0x40000000-0x40000001**: Zeroes all 4 output registers. Normal hypervisors return vendor strings here (e.g., "Microsoft Hv") → eliminated.

### TransparentHideDebugger

Activation function that:
1. Validates callback table (up to 11 non-null function pointers)
2. Copies table to global via SSE2 movups
3. Saves Denuvo's CPUID query context
4. Sets transparency active flag

### TransparentCheckAndTrapFlagAfterVmexit

Handles Trap Flag (TF) in RFLAGS. Denuvo uses single-step debugging to verify code integrity. The function routes `#DB` to the original handler, preserving Denuvo's self-debugging while remaining transparent.

## Security Impact

| Security Control | While HV Active |
|-----------------|----------------|
| Driver Signature Enforcement | **Disabled** |
| Memory Integrity (HVCI) | **Disabled** |
| Credential Guard | **Disabled** |
| Windows Hypervisor (Hyper-V) | **Disabled** |
| KVA Shadow (Meltdown) | **Disabled** (older Intel) |
| Ring -1 | **DenuvOwO HV** (unsigned code controls all hardware) |

The hypervisor has complete control: read/write any memory, intercept all syscalls, hide processes, keylog, network sniff. The included source shows only Denuvo bypass functionality is implemented — no malicious payloads found.

Cleanup is reversible via the tracking registry key. DSE re-enables automatically after reboot. The hypervisor unloads when the game closes.

---

# Appendix A — Denuvo Hardware Fingerprinting Summary

| Vector | What's Checked |
|--------|---------------|
| CPUID leaf 1 | Feature bits (ECX bit 31 = hypervisor present) |
| CPUID 0x80000001 | Extended features |
| CPUID 0x80000002-4 | Processor brand string |
| CPUID 0x40000000-1 | Hypervisor vendor (Hyper-V, VMware, KVM) |
| KUSER_SHARED_DATA | Windows version, build, processor features, architecture |
| PEB | OS version, subsystem version |
| NTDLL | Function bytes, Image Data Directory fields |
| SYSCALL 0x36 | NtQuerySystemInformation |
| XGETBV | Extended control registers |
| GetWindowsDirectoryW | Windows path |
| GetVolumeInformationW | Filesystem/volume info |
| GetComputerNameW | NetBIOS name |
| GetUsernameW | Current username |

# Appendix B — Hypervisor Anti-Detection Techniques

| Technique | What It Defeats |
|-----------|----------------|
| Clear CPUID.1:ECX bit 31 | HV presence detection |
| Zero CPUID 0x40000000 regs | HV vendor fingerprinting |
| TSC spoofing | Timing-based VM detection |
| EPT split pages (exec=read, read=original) | Code integrity self-checks |
| LSTAR/EFER syscall hook | Syscall-level tamper detection |
| Trap Flag forwarding | Single-step integrity verification |
| CR3 monitoring | Process-context-aware memory layout |
