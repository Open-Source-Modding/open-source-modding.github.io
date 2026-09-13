Here is a tutorial on how to change the clothes of an NPC  
first we want to remove for example the balaclava from the chosen one

you need to convert the install pkg file and then go to this "graphics \\ \_common \\ characters \\ fc5 \\ wardrobe \\ \_dna \\ cult \\ chosen file path"

And then there you find this file BWSK and you convert it (with FCBconverter) into an xml file (with note pad)

![][image1]

Then you need to create a new file containing your mod, you will copy the folder containing the BWSK file and paste it into your new folder

![][image2]  
You must now create two files, "info.xml" and "info replace.xml"

![][image3]

open the info.xml file and you must write this  so that the mod knows where the file is with the path (if you do not know how to write with the different combinations, you can help yourself by going to another mod having a file info.xml and copy paste to your file and modify)

![][image4]

now that you have written this no need to open the info\_replace.xml again because for now we want to remove the balaclava, now open this file with notepad

![][image5]

You are in the file, now to remove the balaclava from the chosen one you can do CTRL \+ F to open "search" now write "headwe" and enter

![][image6]

you have to find this path line with the file having .xbg which contains the balaclava

![][image7]

to delete click where i put the red dot and you have to follow the red line to the side and hit its limit, then you have to select the whole paragraph and delete

![][image8]

![][image9]

now that you have removed the paragraph, remove the empty space

![][image10]

save the file and convert it by putting it in "FCBConverter.exe" and delete the old BWSK file and rename your new file to remove the word "\_new"

![][image11]  
now select all files and putting them in a compressed file and choose a name and at the end put ".a3"

![][image12]

put the .a3 file in the modified files folder there is the RM mod and install your mod

![][image13]

the chosen not have a balaclava, now we are going to put a beard on him

![][image14]

To add a beard, take a paragraph containing the beard as on the bwsk file of the VIP chemist, convert it and open the xml file

![][image15]

do as before, do CTRL \+ F and write "facial hair" in search and you will find it, always the same the thing select the paragraph begin and until to the limit of the red line on the side and select the paragraph and copy

![][image16]

open the chosen xml file, have an empty space after the end of a paragraph where I put the red dot and paste the

![][image17]

just in case, remove the small void space if you have one

![][image18]

![][image19]

now like the other time, save, quit, convert it to a BWSK file by going to put it in "FCBConverter.exe" and then remove the old BWSK file and rename your new BWSK file by removing the "\_new"

Now open the file "info\_replace.xml"

![][image20]

Write all of this and if you don't succeed, help yourself by looking at an info\_replace.xml file of an outfit mod, copy and paste into your "info\_replace.xml" file and modify

![][image21]

for that, go to info\_xml and copy that

![][image22]

and go back in info\_replace.xml and paste here

![][image23]

now go to the xml file of the Chosen and select this line of the path file of the paragraph you have added the beard

![][image24]

go back in info\_replace.xml and paste here and save

![][image25]

after saving, like the other time, select all files, turn them into compressed file with .a3 and put it in the modified FC5 files and remove the old mod creates and install

the choosen have an beard, now you know how to add and remove clothes, hair, beards, if you want to add I advise you to take them from a bwsk NPC file not directly from a bwsk object file because it can cause crashes

![][image26]

Enjoy \! 

Author:Bigvadrouiller


























