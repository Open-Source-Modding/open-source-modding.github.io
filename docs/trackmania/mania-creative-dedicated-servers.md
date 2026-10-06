# Mania-Creative — Dedicated Servers and XAseco

> **Source**: Community tutorials originally published on `tutorials.mania-creative.com`, recovered from [web.archive.org](https://web.archive.org/). The dedicated server tutorial was captured on 2011-08-06 and 2011-12-23, and the XAseco tutorial on 2011-01-23 and 2011-12-22. The text below reflects the 2010 to 2013 era of TrackMania (TMF/United and Nations Forever), with the newer wording from the `tm_` captures preferred where the two versions differ. Hostnames, download links, ports and version numbers are kept exactly as the tutorial stated them.

A dedicated server is a computer or a piece of software that is used solely for one purpose. For TrackMania it runs the server independently of the game client, which makes it a good platform for publishing newly made tracks without tying up a machine that is also being played on. This guide follows the two original tutorials: setting up the Nadeo dedicated server, and installing XAseco on top of it.

## Required software

- A text editor. Notepad is enough, or Notepad++ from `notepad-plus.sourceforge.net`.
- A ZIP decompressor. 7zip from `7-zip.org` is the one the tutorial recommends.

## Installing the dedicated server

1. Download the Dedicated Server Software from the Nadeo servers. The tutorial points at the link titled "Actual Server Version (TM-Forum)" at `http://www.tm-forum.com/viewtopic.php?f=28&t=14203`.
2. Decide where to put it. If TrackMania is also installed, the contents of the archive can be placed in the same folder as the game, because the folder structures are identical and both then share the common resources. Otherwise use a separate folder.

The server is configured by editing `\Gamedata\Config\dedicated_cfg.txt` with a text editor.

## Configuring dedicated_cfg.txt

Back up `dedicated_cfg.txt` before changing it. The file is tag-based, so the snippets below are shown as XML.

### Server accounts

The config carries several access levels. Each has a name and a password; the tutorial uses `xxx` as a placeholder password.

```xml
<name>SuperAdmin</name>
<password>xxx</password>
```

The same pattern is repeated for `Admin` and for `User`. XAseco later reuses the SuperAdmin account data to access the server controls, so keep these values at hand.

### Creating a server account

The server logs on to TrackMania with a user account of its own. Create a new, free user account for it. Without a separate account the server cannot run and the same account cannot be used to play TrackMania at the same time.

The Land/Zone of that account becomes the location where the server appears in the online server listing. XAseco can later change the Nation.

For TMUF (TrackMania United Forever) users the tutorial points at `http://official.trackmania.com/tmf-dedicated/`. Log in there with the Multiplayercode (printed on the back of the written manual, or emailed if the game was bought online), click Add, and create the TMUF Dedicated Server Account. Hint from the tutorial: the regular password may be rejected, so try another one.

### Master server account

```xml
<masterserver_account>
    <login>Account</login>
    <password>Password</password>
    <validation_key></validation_key>
</masterserver_account>
```

Leave `validation_key` empty. Common servers do not need it.

### Server options

```xml
<server_options>
    <name>DESIRED SERVERNAME</name>
    <comment></comment>
    <hide_server>0</hide_server>
</server_options>
```

`hide_server` set to `0` means the server is always visible. Text formatting is allowed in the name and comment.

Other options in the same area:

- `max_players` 32
- `<password></password>` (leave empty for an open server)
- `max_spectators` 2
- `<password_spectator></password_spectator>`
- `<ladder_mode>forced</ladder_mode>` forces a permanent connection to Nadeo's master server, which manages the ladder and points.

### Network settings

This block is important, and the tutorial warns to back up `dedicated_cfg.txt` before touching it.

```xml
<force_ip_address></force_ip_address>
<server_port>2352</server_port>
<server_p2p_port>3452</server_p2p_port>
<client_port>0</client_port>
<bind_ip_address></bind_ip_address>
<use_nat_upnp></use_nat_upnp>
<p2p_cache_size>600</p2p_cache_size>
<xmlrpc_port>5002</xmlrpc_port>
<xmlrpc_allowremote>false</xmlrpc_allowremote>
<packmask>stadium</packmask>
```

- `xmlrpc_allowremote` is `false` by default. Set it to `true` only if XAseco, Fast or another tool accesses the server from outside.
- `packmask` is `stadium`. TMUF users should delete the `stadium` value.
- Change the ports so they do not clash with the ports the TrackMania game itself uses.
- The Router/Firewall must pass the new ports, otherwise nobody can see the server online.

Symptom: if the default ports are kept, the server will not run properly because the game is using the same ports.

## Creating the track list and match settings

Log into the game and go online. The "create" button opens the game's internal, simple server menu. If the server software was placed in the game folder, TrackMania automatically copies the values from `dedicated_cfg.txt` onto this settings screen. Check and change them, then press "Launch".

To build the track list, either pick from the predefined and previously saved lists in the middle of the screen, or browse the harddisk folders in the left menu and select single or multiple tracks.

The tutorial's recommendation:

1. In `C:\..\My documents\Trackmania\Tracks\Challenges`, create a subfolder named "Servertracks".
2. Copy all candidate tracks into it.
3. Pick the tracks from that folder before creating or saving lists.

Pressing "Play" starts the game's internal server, not the dedicated one. Instead, select the tracks and save the list with the "Save settings" button, naming it, for example, `MyServerList.txt`, then close Trackmania. Track listings can be changed or expanded on an online server, but only after XAseco is installed.

## Last steps and launching

1. Copy the whole "Servertracks" folder into `[Server-Programm-Ordner]\Tracks\Challenges\`. Nadeo never updated the server software to read the My documents folder, so the tracks must be copied folder to folder.
2. Copy `MyServerList.txt` from `My Documents\Trackmania\Tracks\Matchsettings\` into `[Server-Programm-Ordner]\Tracks\Matchsettings\`.
3. Create a desktop shortcut to `TrackmaniaServer.exe`, open its Properties, and append two parameters to the Target field, a blank space between the path and each parameter:

```ini
C:\TMF\TrackmaniaServer.exe /dedicated_cfg=dedicated_cfg.txt /game_settings=MatchSettings/MyServerList.txt
```

Click the shortcut to start the server. If something is wrong, the console window prints the error. The most likely cause is a file that was copied to the wrong folder.

## Installing XAseco

Aseco stands for Automatic SErver COntrol. It saves records and provides player and admin commands. XASECO is "Xymph's Aseco", which combines the old Aseco with RASP, a plugin set.

### Prerequisites

- The dedicated server itself, configured and working.
- XAMPP, which supplies PHP and MySQL.
- The XAseco files.
- The login data from `dedicated_cfg.txt`: the SuperAdmin name and password, the server login and password, the Nation, and the XML-RPC port.

Download Xaseco at `http://gamers.org/tmn/` and XAMPP at `http://garr.dl.sourceforge.net/sourceforge/xampp/xampp-win32-1.6.7-installer.exe`. Unpack the `*.rar` with WinRAR. Install XAMPP and mark "Install MySQL" during setup.

### Starting MySQL and Apache

Open the XAMPP Control Center. MySQL should show "running"; if not, press start. Then start Apache, which should also show "running". If "running" only blinks for a moment, then Skype is running; close Skype and retry.

In a browser, open `127.0.0.1` and choose your language. Go to `http://localhost/security/xamppsecurity.php`, create a password and remember it. Then create a second account, set the user to `root`, retype the password, and save.

### Placing the XAseco files

In the folder `xaseco`, enter the subfolder `newinstall` and copy all of its files into `xaseco`, with three exceptions. These three go into the `includes` folder instead:

- `jfreu.config.php`
- `rasp.settings.php`
- `votes.config.php`

### Editing config.xml

Open `config.xml` with a text editor (Notepad, OpenOffice, Word or Notepad++).

The `masteradmins` block determines who is a MasterAdmin. The comment in the file notes that the `ip:port` in `tmlogin` is only needed when joining over LAN, and that `ipaddress` guards each login against unauthorized use of admin commands from other IPs.

```xml
<masteradmins>
    <tmlogin>no_lolmaps</tmlogin>
    <ipaddress></ipaddress>
</masteradmins>
```

Replace `no_lolmaps` with your own TrackMania login to become MasterAdmin. The `ipaddress` entry is a security feature: it is the IP the account may log in from, and other IPs are rejected.

The `colors` block holds the colours XAseco displays:

```xml
<colors>
    <error>$f00$i</error>
    <welcome>$f00</welcome>
    <server>$ff0</server>
    <highlite>$fff</highlite>
    <timelite>$bbb</timelite>
    <record>$0f3</record>
    <emotic>$fa0</emotic>
    <music>$d80</music>
    <message>$39f</message>
    <rank>$ff3</rank>
    <vote>$f8f</vote>
    <karma>$ff0</karma>
    <donate>$f0f</donate>
    <admin>$ff0</admin>
    <black>$000</black>
    <grey>$888</grey>
    <login>$00f</login>
    <nick>$f00</nick>
    <interact>$ff0$i</interact>
    <dedimsg>$28b</dedimsg>
    <dedirec>$0b3</dedirec>
</colors>
```

At the end of `config.xml` is the `tmserver` block. Its values must match `dedicated_cfg.txt`.

```xml
<tmserver>
    <login>SuperAdmin</login>
    <password>YOUR_SUPERADMIN_PASSWORD</password>
    <ip>127.0.0.1</ip>
    <port>5000</port>
</tmserver>
```

The tutorial notes that the port of its dedicated server was 5002, and says to look it up and enter whatever you actually chose. The IP `127.0.0.1` means the server runs on your own PC.

### Editing localdatabase.xml

```xml
<localdatabase>
    <mysql_server>localhost</mysql_server>
    <mysql_login>YOUR_MYSQL_LOGIN</mysql_login>
    <mysql_password>YOUR_MYSQL_PASSWORD</mysql_password>
    <mysql_database>aseco</mysql_database>
</localdatabase>
```

`mysql_login` must be `root`, and the password is the one created earlier. The database name is `aseco` (really `aseco`, not `xaseco`).

Two further settings in the same file:

- `<display>true</display>` controls whether newly driven records are shown. Set it to `false` to disable them.
- `<limit>50</limit>` is the number of records publicly displayed, not the total number tracked. Enter a number or leave it empty.

### Editing aseco.bat

Right-click `aseco.bat` and edit it so the PHP path points at the XAMPP installation:

```ini
@echo off
rem ****** Set here your php path *******
set INSTPHP=C:\Programme\Apache2\Php5
rem *************************************
PATH=%PATH%;%INSTPHP%;%INSTPHP%\extensions
"%INSTPHP%\php.exe" aseco.php
pause
```

After `set INSTPHP=`, put the XAMPP install folder, for example `set INSTPHP=D:\Programme\xampp\php`.

### Editing dedimania.xml

Open `dedimania.xml` and find the `masterserver_account` block. Enter the data from `dedicated_cfg.txt`.

```xml
<masterserver_account>
    <login>YOUR_SERVER_LOGIN</login>
    <password>YOUR_SERVER_PASSWORD</password>
    <nation>YOUR_SERVER_NATION</nation>
</masterserver_account>
```

### Importing the database

In the browser, go to `127.0.0.1` and open phpMyAdmin. Enter the login information and confirm. On the new page, type `aseco` in the field next to the left arrow and click the button next to the right arrow. On the page that appears, hit import, browse to `aseco.sql` in the `xaseco/localdb` folder, and confirm. Repeat the import with `rasp.sql`. Close the browser.

### Starting XAseco

Start the TrackMania server first, then double-click `aseco.bat` in the `xaseco` folder. If the tutorial was followed, no error appears.

## Common failure symptoms

- The server will not run properly with the default ports, because the TrackMania game is using the same ports.
- Nobody can see the server online when the Router/Firewall does not pass the configured ports.
- The server console prints an error on launch, most likely because a track list or track folder was copied to the wrong place.
- XAMPP's Apache or MySQL "running" status only blinks; Skype is the usual cause, and closing it fixes the problem.
- Admin commands are refused when the connecting IP does not match the `ipaddress` set for that MasterAdmin.
- XAseco does not track records if `mysql_login`, `mysql_password` or the `aseco` database name are wrong, or if the `aseco.sql` and `rasp.sql` imports were not run.

## A note on the current ecosystem

Everything above describes the 2010 to 2013 TrackMania Forever/United era. The dedicated server software, the XAMPP version, the XAseco download host and the Nadeo account pages have all changed since; the linked hosts in particular are no longer the places to get current software. Treat this page as a historical reference for how the setup was done, not as current download instructions.

## Credits

- Dedicated server tutorial: **SmashingDeluXe**. Translations by TStarGermany and Vallandil. Last updated April 28, 2011 (older capture) and September 02, 2011 (`tm` capture).
- XAseco tutorial: **Fool**. Translations by Fool. Last updated January 14, 2011 (older capture) and September 02, 2011 (`tm` capture).

Related pages: [File and asset reference](mania-creative-file-and-asset-reference.md), [Manialinks](mania-creative-manialinks.md), [Text formatting and charset](mania-creative-text-formatting-and-charset.md).
