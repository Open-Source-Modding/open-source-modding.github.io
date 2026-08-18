# GitHub - figment/hkxcmd: Havok Command Line Tools for Skyrim · GitHub

> Source: https://github.com/figment/hkxcmd
> Cached: 2026-08-18T19:27:19.739Z

---

figment
    
    /
    
      [hkxcmd](/figment/hkxcmd)
    

    Public
  

        

        
            

    
      

  
                
    
Notifications
    You must be signed in to change notification settings

  

  
              
    
Fork
    27

  

  
        
            
    

          Star
          69

  

        
      

        

          

  

      
  
    
              
    

        Code
          

    

      
  
    
              
    

        Issues
          4

    

      
  
    
              
    

        Pull requests
          1

    

      
  
    
              
    

        Actions
          

    

      
  
    
              
    

        Projects
          

    

      
  
    
              
    

        Wiki
          

    

      
  
    
              
    

        Security and quality
          0

    

      
  
    
              
    

        Insights
          

    

          
  
      
    

Additional navigation options

  
    
                
  
    

        
    
    
    
        
          
    

        
      
        
          Code
      

  

        
    
    
    
        
          
    

        
      
        
          Issues
      

  

        
    
    
    
        
          
    

        
      
        
          Pull requests
      

  

        
    
    
    
        
          
    

        
      
        
          Actions
      

  

        
    
    
    
        
          
    

        
      
        
          Projects
      

  

        
    
    
    
        
          
    

        
      
        
          Wiki
      

  

        
    
    
    
        
          
    

        
      
        
          Security and quality
      

  

        
    
    
    
        
          
    

        
      
        
          Insights
      

  

    

      
  

  
  

    
    

    
      
    

  
  
    [](/figment/hkxcmd) master[Branches](/figment/hkxcmd/branches)[Tags](/figment/hkxcmd/tags)[](/figment/hkxcmd/branches)[](/figment/hkxcmd/tags)Go to fileCodeOpen more actions menu## Latest commit

 ## History

[10 Commits](/figment/hkxcmd/commits/master/)[](/figment/hkxcmd/commits/master/)10 Commits## Folders and files

NameNameLast commit messageLast commit date[Addins](/figment/hkxcmd/tree/master/Addins)[Addins](/figment/hkxcmd/tree/master/Addins)  [Core](/figment/hkxcmd/tree/master/Core)[Core](/figment/hkxcmd/tree/master/Core)  [Loki](/figment/hkxcmd/tree/master/Loki)[Loki](/figment/hkxcmd/tree/master/Loki)  [compat](/figment/hkxcmd/tree/master/compat)[compat](/figment/hkxcmd/tree/master/compat)  [zlib](/figment/hkxcmd/tree/master/zlib)[zlib](/figment/hkxcmd/tree/master/zlib)  [LICENSE.TXT](/figment/hkxcmd/blob/master/LICENSE.TXT)[LICENSE.TXT](/figment/hkxcmd/blob/master/LICENSE.TXT)  [README.txt](/figment/hkxcmd/blob/master/README.txt)[README.txt](/figment/hkxcmd/blob/master/README.txt)  [hkxcmd_VC90.sln](/figment/hkxcmd/blob/master/hkxcmd_VC90.sln)[hkxcmd_VC90.sln](/figment/hkxcmd/blob/master/hkxcmd_VC90.sln)  [hkxcmd_VC90.vcproj](/figment/hkxcmd/blob/master/hkxcmd_VC90.vcproj)[hkxcmd_VC90.vcproj](/figment/hkxcmd/blob/master/hkxcmd_VC90.vcproj)  [makeconfig.bat](/figment/hkxcmd/blob/master/makeconfig.bat)[makeconfig.bat](/figment/hkxcmd/blob/master/makeconfig.bat)  View all files## Repository files navigation

- 
- [README](#)
- [License](#)

More items        Havok Command Line Tools 1.4
        ============================

This package includes a command line application for converting Havok models 
from Skyrim the native format to something that can be manipulated or used as 
a reference for non-derivative works.
      
Currently the package only converts from native HKX files built with 
Havok 2010.2.0 SDK to the XML form of HKX or to the Gamebryo KF file format.

   
Prerequisites:
  * For Skyrim, you must unpack the BSA files using an appropriate tool.
    For me I will refer to this location as "%TEMP%\Skyrim - Animations"
  
  * I assume you have the 2010.2 Havok Content Tools and SDK from Intel.
    You will need to provide infomation to get access to the tools but is free.
    [http://software.intel.com/sites/havok/en/](http://software.intel.com/sites/havok/en/)
    
  * For 3ds max support get the niftools.org exporter for Skyrim
    - [http://www.niftools.org](http://www.niftools.org)
    - [http://niftools.sourceforge.net/forum/viewtopic.php?f=20&t=3178](http://niftools.sourceforge.net/forum/viewtopic.php?f=20&t=3178)
  
Quick Start:
  1. Convert Packed HKX to XML HKX files
    * If you do not specify the output folder it will assume that it should write to 
        the same folder and append "-out" the filename.
    * The XML HKX files can be loaded into Havok tools like the Havok Preview Tool    
    > hkxcmd convert "%TEMP%\Skyrim - Animations"
      
  2. Convert Packed HKX to XML HKX files and place in another folder
    * The following will convert all HKX files in the "%TEMP%\Skyrim - Animations" folder
        and subfolders and place them in the "%TEMP%\Skyrim - Animations - Output" folder. 
    > hkxcmd convert "%TEMP%\Skyrim - Animations" "%TEMP%\Skyrim - Animations - Output"

  3. Preview Animation in Havok Preview Tool
    * Load the Preview Tool: Start | Havok | Havok PcXs Content Tools 2010.2.0 | Havok Preview Tool
    * Convert Skeleton.hkx using step 1 or 2 above
    * File | Open... | Browse for converted skeleton
      - Assuming you used Tutorial 2 that would be the following for the chicken      
         "%TEMP%\Skyrim - Animations - Output\meshes\actors\ambient\chicken\character assets\skeleton.hkx"
      - If you get a "Wrong platform for packfile" error when loading then you selected the original skeleton
        not the converted one.  
        
    * File | Add... | Browse for an animation for that skeleton
      - Assuming you used Tutorial 2 that would be the following for the chicken 
         "%TEMP%\Skyrim - Animations - Output\meshes\actors\ambient\chicken\animation\getup.hkx"

  4. Convert HKX file to Gamebryo KF files
    * The following will just dump KF files into same folder as the HKX file
    > hkxcmd exportkf "%TEMP%\Skyrim - Animations"
    
  5. Convert HKX file to Gamebryo KF files into a different folder
    * The following will convert all HKX files in the "%TEMP%\Skyrim - Animations" folder 
        and subfolders and place them in the "%TEMP%\Skyrim - Animations - Output" folder. 
    > hkxcmd exportkf "%TEMP%\Skyrim - Animations" "%TEMP%\Skyrim - Animations - Output"

  6. Import Skyrim Animation into 3ds Max
    * First import the creature into max using niftools max importer
      - Select Import | "Netimmerse/Gamebryo (*.nif, *.kf)" | Pick the file to import
       * Continuing the chicken theme I used the following
       * "%TEMP%\Skyrim - Meshes\meshes\actors\ambient\chicken\character assets\chicken.nif"      
      - If you have problems with meshes try importing the skeleton explicitly before the mesh     
        * Not all creatures use the name skeleton.nif so you might have to manually adjust path
       
    * Assuming this succeeded now import the animation
      - Select Import | "Netimmerse/Gamebryo (*.nif, *.kf)" | Pick the animation
      - Find an animation you which to import such as the getup animation
        * Assuming you used Tutorial 5 that would be the following for the chicken:
         "%TEMP%\Skyrim - Animations - Output\meshes\actors\ambient\chicken\animation\getup.kf"
         
  7. Export Skyrim Animation from 3ds Max in KF format
    * Select the root node (usually "NPC Root [Root]")
    * Open Graph Editors | Track View - Dope Sheet
    * Add a Note Track to the Root Node
    * Note that the KF generated by the exportkf process will now add start and end note tracks
    * Add a note at time 0 with text "start"
    * Add a note on the final frame of animation with text "end"
    * Select Export | "Netimmerse/Gamebryo (*.nif, *.kf)" | Specify an file name like "export.kf"
    * Select "Single KF w/o NIF" under Animation in the "Export Nif" dialog
    * Select "Export"
    * I usually load in Nifskope after this just to make sure its valid enough

  8. Convert KF format to HKX Format
    * The following will convert a KF file using the base skeleton for its bone layout and bind pose
    * By default it exports as a Win32 Packed Binary HKX file but you can 
      change that from the command line to export as XML adding -v:XML
    > hkxcmd convertkf "\path\to\skeleton.hkx" "\path\to\export.kf" "\path\to\export.hkx"
    
Troubleshooting:
  - A lot of files show that they failed to load when converting.
    * Ignore any errors about files it cannot convert as not every file 
      can be read with the sdk. This is because the SDK only includes Physics and Animation
      support. Bethesda also uses Havok Behaviors which are not supported by the SDK
      and those file cannot be read currently.

  - I keep getting the following message "Wrong platform for packfile" when using the Preview tool
  
    * You are loading the original animations from Bethesda which are not directly 
      compatible with the preview tool.  Follow the tutorial parts 1 and 2 and 
      export the XML HKX files and use those.
    
  - The animation is corrupt after importing into 3ds Max
    * Try Importing the Skeleton.NIF file explicitly before importing the mesh.  
      Dont import skeleton again when loading the mesh when doing this.
    * Some animations just don't import correctly probably because of bone mapping issues

  - When I try to load the converted HKX file into Havok Preview Tool fails.
    * The Havok Preview Tool breaks with large XML files like the dragon.hkx files.
    * You might be able to convert the file to a packed file and load that but does not work for me

Known Issues:

  - I could not figure out how bones are mapped because I'm not sure the information is
    present.  I've just assumed that there is a 1-to-1 skeleton to animation track
    but that is obviously not always a correct assumption

  - Havok supports non-uniform scale in animation however Gamebryo does not
    This means any animation exported to the Gamebryo format may lose 
    information when this is present.
  
  - KF was choosen because the niftools.org 3ds max importer largely works
    and this was the easiest way to do this porting.  It may be worth while
    converting to FBX format instead but that is much more work but
    would presumably have broader support.
    
  - There is no handling for visbility or camera tracks which seem to be used
    
Change Log:
  1.4
   * Add support for Havok Behavior files and Bethesda specific classes
   * Altered ExportKF to used frame_increment = duration / (nframe-1);
   * Use a round function when calculated frames in ConvertKF
  1.3
   * Change ExportKF to add "start" and "end" note tracks on root node for later export
   * Added ConvertKF which takes a Gamebryo KF file and produces an HKX file
   * Added DumpText which is a text file dump of bones in a skeleton may be useful in 
     the Havok Export Pipeline when specifying the skeleton.
   * Added Retarget is intended to fix animation to match another skeleton bone layout
     (Does not currently work correctly)
     
  1.2
   * Redid the directory handling for exportkf 
  1.1
   * Fixed crash when no output folder was specified when using the exportkf command
  
  
Source Code:
  * Source is hosted at github.org
    - git://github.org/figment/hkxcmd.git
    - git://github.org/figment/niflib.git
  
Acknowledgements:
  * Niftools team for niflib and the 3ds Max Importer
  * Havok and Intel for the Havok SDK and perpetual license for 3rd party tools
  ## About

Havok Command Line Tools for Skyrim

### Resources

[Readme](#readme-ov-file)[License](#License-1-ov-file)[Activity](/figment/hkxcmd/activity)### Stars

**69** stars### Watchers

**13** watching### Forks

[**27** forks](/figment/hkxcmd/forks)[Report repository](/contact/report-content?content_url=https%3A%2F%2Fgithub.com%2Ffigment%2Fhkxcmd&report=figment+%28user%29)## Releases

## Packages

## Contributors

## Languages