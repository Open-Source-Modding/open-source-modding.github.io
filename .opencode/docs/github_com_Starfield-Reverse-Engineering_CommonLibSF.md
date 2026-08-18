# GitHub - Starfield-Reverse-Engineering/CommonLibSF: A collaborative reverse-engineered library for Starfield · GitHub

> Source: https://github.com/Starfield-Reverse-Engineering/CommonLibSF
> Cached: 2026-08-18T19:30:21.631Z

---

# CommonLibSF

[](#commonlibsf)
# This repo is unmaintained, please go to [libxse/commonlibsf](https://github.com/libxse/commonlibsf) for updates

[](#this-repo-is-unmaintained-please-go-to-libxsecommonlibsf-for-updates)
## A collaborative reverse-engineered library for Starfield

[](#a-collaborative-reverse-engineered-library-for-starfield)
[](https://en.cppreference.com/w/cpp/compiler_support)
[](https://camo.githubusercontent.com/187e1ca14492715e6296dc0fc7a7b5325f9225dfcbfb2893a2d0f6ee10452937/68747470733a2f2f696d672e736869656c64732e696f2f7374617469632f76313f6c6162656c3d706c6174666f726d266d6573736167653d77696e646f777326636f6c6f723d64696d67726179267374796c653d666c6174266c6f676f3d77696e646f7773)
[](#Developing-with-CommonLibSF)
[](https://github.com/Starfield-Reverse-Engineering/CommonLibSF/actions/workflows/main_ci.yml)
## Requirements

[](#requirements)

[CMake v3.30+](https://cmake.org/) or [XMake v2.8.2+](https://github.com/xmake-io/xmake/releases)

- Add this to your `PATH`

[vcpkg](https://github.com/microsoft/vcpkg)

- Create a Windows environment variable called `VCPKG_ROOT` with the value as the path to your vcpkg install directory

[Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads)

- [LLVM](https://releases.llvm.org/) if using `clang-cl` presets

- [PowerShell](https://github.com/PowerShell/PowerShell/releases)

## Developing with CommonLibSF

[](#developing-with-commonlibsf)
### Using the CommonLibSF plugin templates

[](#using-the-commonlibsf-plugin-templates)

[CLibSFPluginTemplate](https://github.com/Starfield-Reverse-Engineering/CLibSFPluginTemplate)

- Uses **CMake**. Consumes CommonLibSF as [vcpkg port](https://github.com/Starfield-Reverse-Engineering/CommonLibSF-vcpkg).

[commonlibsf-template-xmake](https://github.com/Starfield-Reverse-Engineering/commonlibsf-template-xmake)

- Uses **XMake**. Consumes CommonLibSF as an [xrepo package](https://github.com/Starfield-Reverse-Engineering/commonlibsf-xrepo), a git submodule, or a local fork.

### Please see the [Using CommonLibSF wiki page](https://github.com/Starfield-Reverse-Engineering/CommonLibSF/wiki/Using-CommonLibSF) for more information about using CommonLibSF.

[](#please-see-the-using-commonlibsf-wiki-page-for-more-information-about-using-commonlibsf)
## End-User Requirements

[](#end-user-requirements)

- [Starfield Script Extender](https://www.nexusmods.com/starfield/mods/106)

- [Address Library for SFSE Plugins](https://www.nexusmods.com/starfield/mods/3256)

## Notes

[](#notes)
CommonLibSF is intended to replace SFSE as a static dependency. However, the runtime component of SFSE is still required.

You may download the PDB for the latest build of CommonLibSF via the [Update PDB](https://github.com/Starfield-Reverse-Engineering/CommonLibSF/actions/workflows/update_pdb.yml) workflow. This is useful for importing the latest type information into IDA using the "Import PDB" feature.

## License

[](#license)
[GPL-3.0-or-later](/Starfield-Reverse-Engineering/CommonLibSF/blob/main/COPYING) WITH [Modding Exception AND GPL-3.0 Linking Exception (with Corresponding Source)](/Starfield-Reverse-Engineering/CommonLibSF/blob/main/EXCEPTIONS).

Specifically, the **Modded Code** is Starfield (and its variants) and **Modding Libraries** include [SFSE](https://sfse.silverlock.org/) ([GitHub link](https://github.com/ianpatt/sfse)).

It is recommended to read the [FSF FAQ](https://www.gnu.org/licenses/gpl-faq.en.html) for a full explanation on what this license requires.

The main requirement of this license is: if you link your mod against CommonLibSF, and you distribute the binaries for that mod, you must also distribute the source code under the same license as CommonLibSF along with the source code for any other libraries you link against, with the exception of the Modded Code and Modding Libraries defined above.

## Contributors

[](#contributors)

  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  
  
    
      
    
  

## Credits

[](#credits)
This work is based on [CommonLibSSE](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE) created by [Ryan-rsm-McKenzie](https://github.com/Ryan-rsm-McKenzie) under an [MIT License](/Starfield-Reverse-Engineering/CommonLibSF/blob/main/CommonLibSF/LICENSES/COMMONLIB). Without him putting that together, much of the joint Bethesda RE scene would be drastically different. The MIT License is kept for historical purposes only and the [GPL](#License) governs this work. To the extent that you want the [MIT licensed code](/Starfield-Reverse-Engineering/CommonLibSF/blob/main/CommonLibSF/LICENSES/COMMONLIB), please reference [CommonLibSSE and its forks](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE).