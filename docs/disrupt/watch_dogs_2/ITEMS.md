# WATCH DOGS 2

JamesLurk — 10/26/23, 15:57
Hey slick, share with me your project that adds new items. I'm afraid you'll never release it, and it's the biggest modification I've seen in WD2 so far
the task of the Pesky Fly — 10/26/23, 15:57
:aiding:
JamesLurk — 10/26/23, 15:58
hahaha
the task of the Pesky Fly — 10/26/23, 16:01
Adding tutorial:
Find the model you want in graphickit_models
Find the part you want by comparing with graphickit_parts
Create a new duplicate entry in graphickit_parts, give it unique IDs and erase all model overrides except one
Replace the override with your own, hashed filepath
Create new items.lib entry, give it unique IDs, point it to custom graphickit_parts entry
Extract tagcategories.lib and tags.lib
Go back to the graphickit_parts entry
Find the store tag category inside tagcategories.lib
Find a matching store tag in tags.lib you wish to use
Replace the FF... store tag in graphickit_parts entry with the store you selected
Create a new itemshopproperties.lib entry, give it unique IDs and point it to your items.lib
Edit shopcomponentsettings.lib and add the item to desired shop
 
Oh right I also created my own variants
Adding tutorial:
Extract the original texture and model
Locate the original material.bin
Duplicate the material bin and point it to new texture path
Locate the original .model file
Duplicate the .model file
Hex edit the .model file to point to new material
Find the base model you want in graphickit_models
Find the base part you want by comparing with graphickit_parts
Create a new duplicate entry in graphickit_parts, give it unique IDs and erase all model overrides except one
Replace the override with your own .model, hashed filepath
Create new items.lib entry, give it unique IDs, point it to custom graphickit_parts entry
Extract tagcategories.lib and tags.lib
Go back to the graphickit_parts entry
Find the store tag category inside tagcategories.lib
Find a matching store tag in tags.lib you wish to use
Replace the FF... store tag in graphickit_parts entry with the store you selected
Create a new itemshopproperties.lib entry, give it unique IDs and point it to your items.lib
Edit shopcomponentsettings.lib and add the item to desired shop
Oh right I also added a new category--
