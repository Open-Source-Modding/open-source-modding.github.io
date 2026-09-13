**How to use replace in depload**

If you want to add or replace some entries in \*\_depload.dat files, you
can also use info_replace.xml.

Here are some examples.

## How a \*\_depload.dat works?

Depload file, exactly dependency loader, is a file which preloads some
files which are required by another files. Typical example is bwsk. If
you edit bwsk -- change some xbg paths, the game crash when load the
edited bwsk.

So, to prevent the game from crashing you need to change the xbg path
also in depload file.

For example here is depload entry of uni100 bwsk which is Carmina from
New Dawn.

\<CBinaryResourceContainer ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\\_dna\\uniques\\dna_uni100.item.bwsk\"\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\handwear\\handw_uni100_glove01_aver_f.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_uni100_aver_f.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\feet\\feet_uni100_boots01_aver_f.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\headwear\\headw_uni100_beanie01_aver_f.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\top\\top_uni100_jackets_aver_f.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\head\\head_uni100_carminarye_aver_f.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\bottom\\botm_uni100_pants_aver_f.xbg\" /\>

  \<CMaterialResource ID=\"graphics\\\_materials\\psavignac-m-20180511141028.material.bin\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\head\\teeth\\head_shar_teetha_aver_mf.xbg\" /\>

  \<CNomadResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\wardrobe.config.bwsk\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\head\\eyes\\eyes_share_aver_mf.xbg\" /\>

  \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\body\\body_npc_basemesh_aver_f.xbg\" /\>

\</CBinaryResourceContainer\>

It means that *dna_uni100.item.bwsk* requires all these files to be
preloaded. If you delete any child of *CBinaryResourceContainer*, the
game will crash. So, if you change for example *hair_uni100_aver_f.xbg*
in the bwsk, you must change it also here.

The name of each child of *CBinaryResourceContainer* means what kind of
a file it is.

You can of course add as many *\*Resource* nodes as you want -- or how
many are in bwsk.

Sometimes in depload are not all files which are in bwsk, why? I don't
know -- another Ubi strange thing. But for sure, you can put in depload
all files which are in bwsk.

And now, the info_replace.

## Replacing a resource file

If you want to replace a resource file, then you can do:

\<Replace RequiredFile=\"patch_depload.dat\"\>

  \<CBinaryResourceContainer
ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\\_dna\\uniques\\dna_uni100.item.bwsk\"\>

    \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_uni100_aver_f.xbg\" newID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_npc_ponytail01_aver_f.xbg\" newType=\"CGeometryResource\" /\>

  \</CBinaryResourceContainer\>

\</Replace\>

You just put copied *CBinaryResourceContainer* node from converted
depload file. Then you add new *newID* attribute and *newType* attribute
to *\*Resource* node.

This example change dependency on file from:

**graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_uni100_aver_f.xbg**

to

**graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_npc_ponytail01_aver_f.xbg**

## Adding new files / resources

If you want to add new files which are not present in depload, you can
do it like this:

\<Replace RequiredFile=\"patch_depload.dat\"\>

  \<CBinaryResourceContainer ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\\_dna\\uniques\\dna_my_uni.item.bwsk\" addNode=\"1\"\>

    \<CGeometryResource ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\handwear\\handw_uni100_glove01_aver_f.xbg\" /\>

    \<CGeometryResource
ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_uni100_aver_f.xbg\" /\>

  \</CBinaryResourceContainer\>

\</Replace\>

It is basically same as normal *CBinaryResourceContainer* node, but you
just add *addNode* attribute with value 1.

This example will add new entry to depload.

If you want to add only one *\*Resource* to a
*CBinaryResourceContainer*, then do this:

\<Replace RequiredFile=\"patch_depload.dat\"\>

  \<CBinaryResourceContainer ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\\_dna\\uniques\\dna_my_uni.item.bwsk\"\>

    \<CGeometryResource
ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\hair\\hair_uni100_aver_f.xbg\" addNode=\"1\"
/\>

  \</CBinaryResourceContainer\>

\</Replace\>

Simply add attribute *addNode* with value 1.

## Removing \*Resource nodes

Sometimes if you want to remove some resource files, you can do it like
this:

\<Replace RequiredFile=\"patch_depload.dat\"\>

  \<CBinaryResourceContainer
ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\\_dna\\uniques\\dna_uni100.item.bwsk\"\>

    \<CGeometryResource
ID=\"graphics\\\_common\\characters\\bowmore\\wardrobe\\handwear\\handw_uni100_glove01_aver_f.xbg\" deleteNode=\"1\" /\>

  \</File\>

\</Replace\>

This will remove the file
**graphics\\\_common\\characters\\bowmore\\wardrobe\\handwear\\handw_uni100_glove01_aver_f.xbg**
from required files by **dna_uni100.item.bwsk**
