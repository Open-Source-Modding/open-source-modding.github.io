**How to use replace in CombinedMoveFile**

Combined Move File is divided into two big parts. First part contains
some basic data about \*.move.bin files, some kind of headers, and
second part contains FCB data where are defined detailed config entries
for \*.move.bin files.

So, because there are two different types of configs, you must specify
which one you want to replace. First is *PerMoveResourceInfos* and
second is *FCBData*.

Here is an example config entry from *PerMoveResourceInfos*

\<CMove_BlendRoot_DTRoot hash=\"BF4260DED58FDD4A\" rootNodeId=\"6D8384FA47B8E14D\"\>

  \<CMoveTransition space0=\"0\" uniqueID=\"7891297182563230029\" headerValueA=\"0\" headerValueB=\"0\"\>

    \<CMoveTransitionContainer unknownA=\"1\" headerValueA=\"0\" headerValueB=\"0\"\>

      \<CMoveBlendRef unknownA=\"0.33333334\" blendParamOffset=\"4F063E3584000046\" space0=\"0\" layerParamOffset=\"3D1FB74803D60ECB\" space1=\"0\" moveblendParamOffset=\"03AEF5A8C0CAAFA1\" space2=\"0\" headerValueA=\"0\" headerValueB=\"0\" /\>

    \</CMoveTransitionContainer\>

    \<CMoveTransitionContainer unknownA=\"0\" headerValueA=\"0\" headerValueB=\"0\"\>

      \<CMoveBlendRef unknownA=\"0.33333334\" blendParamOffset=\"4F063E3584000046\" space0=\"0\" layerParamOffset=\"3D1FB74803D60ECB\" space1=\"0\" moveblendParamOffset=\"E4707D10A7BB72B2\" space2=\"0\" headerValueA=\"0\" headerValueB=\"0\" /\>

    \</CMoveTransitionContainer\>

    \<CMoveBlendRef unknownA=\"0.33333334\" blendParamOffset=\"4F063E3584000046\" space0=\"0\" layerParamOffset=\"3D1FB74803D60ECB\" space1=\"0\" moveblendParamOffset=\"2CFC9F96B432742C\" space2=\"0\" headerValueA=\"0\" headerValueB=\"0\" /\>

  \</CMoveTransition\>

\</CMove_BlendRoot_DTRoot\>

Now we want to replace a value named *layerParamOffset* in
*CMoveBlendRef* of second *CMoveTransitionContainer*. Because there
aren't any usable ID, we must use *count* attribute, just like in
replacing fcb.

First we must select *CMove_BlendRoot_DTRoot*. Primary key is *hash*
attribute.

Replace will look like this:

\<CMove_BlendRoot_DTRoot hash=\"BF4260DED58FDD4A\"\>

  \<CMoveTransition\>

    \<CMoveTransitionContainer count=\"1\"\>

      \<CMoveBlendRef layerParamOffset=\"our_new_value\" /\>

    \</CMoveTransitionContainer\>

  \</CMoveTransition\>

\</CMove_BlendRoot_DTRoot\>

When replacing, we leave only those attributes which we want to be
replaced, other we can remove.

Now as I wrote, we need to specify the part of combined, so complete
replace will look like this:

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<DefaultInclude\>false\</DefaultInclude\>

  \<Name\>Combined Move File Sample Replace\</Name\>

\<Description\>\</Description\>

  

  \<Replaces\>

    \<Replace RequiredFile=\"move\\combinedmovefile.bin\"\>

      \<PerMoveResourceInfos\>

        \<CMove_BlendRoot_DTRoot hash=\"BF4260DED58FDD4A\"\>

          \<CMoveTransition\>

            \<CMoveTransitionContainer count=\"1\"\>

              \<CMoveBlendRef layerParamOffset=\"our_new_value\" /\>

            \</CMoveTransitionContainer\>

          \</CMoveTransition\>

        \</CMove_BlendRoot_DTRoot\>

      \</PerMoveResourceInfos\>

    \</Replace\>

  \</Replaces\>

\</PackageInfoReplace\>

Note the *PerMoveResourceInfos* node.

## Adding new entries to *PerMoveResourceInfos*

You put whole *CMove_BlendRoot_DTRoot* entry to a replace info file and
add *addNode* attribute with value 1.

Then it will look:

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<DefaultInclude\>false\</DefaultInclude\>

  \<Name\>Combined Move File Sample Replace\</Name\>

\<Description\>\</Description\>

  

  \<Replaces\>

    \<Replace RequiredFile=\"move\\combinedmovefile.bin\"\>

      \<PerMoveResourceInfos\>

        \<CMove_BlendRoot_DTRoot hash=\"BF4260DED58FDD4A\" rootNodeId=\"6D8384FA47B8E14D\" addNode=\"1\"\>

          \<CMoveTransition space0=\"0\" uniqueID=\"7891297182563230029\" headerValueA=\"0\" headerValueB=\"0\"\>

            \<CMoveTransitionContainer unknownA=\"1\" headerValueA=\"0\" headerValueB=\"0\"\>

              \<CMoveBlendRef unknownA=\"0.33333334\" blendParamOffset=\"4F063E3584000046\" space0=\"0\" layerParamOffset=\"3D1FB74803D60ECB\" space1=\"0\" moveblendParamOffset=\"03AEF5A8C0CAAFA1\" space2=\"0\" headerValueA=\"0\" headerValueB=\"0\" /\>

            \</CMoveTransitionContainer\>

            \<CMoveTransitionContainer unknownA=\"0\" headerValueA=\"0\" headerValueB=\"0\"\>

              \<CMoveBlendRef unknownA=\"0.33333334\" blendParamOffset=\"4F063E3584000046\" space0=\"0\" layerParamOffset=\"3D1FB74803D60ECB\" space1=\"0\" moveblendParamOffset=\"E4707D10A7BB72B2\" space2=\"0\" headerValueA=\"0\" headerValueB=\"0\" /\>

            \</CMoveTransitionContainer\>

            \<CMoveBlendRef unknownA=\"0.33333334\" blendParamOffset=\"4F063E3584000046\" space0=\"0\" layerParamOffset=\"3D1FB74803D60ECB\" space1=\"0\" moveblendParamOffset=\"2CFC9F96B432742C\" space2=\"0\" headerValueA=\"0\" headerValueB=\"0\" /\>

          \</CMoveTransition\>

        \</CMove_BlendRoot_DTRoot\>

      \</PerMoveResourceInfos\>

    \</Replace\>

  \</Replaces\>

\</PackageInfoReplace\>

## Replacing existing CMove_BlendRoot_DTRoot with new

There may be a situation when you need to replace an existing node with
a new which has different children nodes. Put these attributes to
*CMove_BlendRoot_DTRoot*: removeNode and addNode with value 1. Example:

        \<CMove_BlendRoot_DTRoot hash=\"BF4260DED58FDD4A\" rootNodeId=\"6D8384FA47B8E14D\"
removeNode=\"1\" addNode=\"1\"\>

## Replacing values in children of CMove_BlendRoot_DTRoot

Replacing values of pretty easy:

\<PerMoveResourceInfos\>

        \<CMoveSingleAnim uniqueID=\"18372017836343257010\" layerParamOffset=\"aaa\"
/\>

      \</PerMoveResourceInfos\>

Mod Installer will search for *CMoveSingleAnim* node with *uniqueID*
that is set in the replace. When successfull, attribute
*layerParamOffset* of the node will be changed to „aaa".

## Deleting values in children of CMove_BlendRoot_DTRoot

\<PerMoveResourceInfos\>

        \<CMoveSingleAnim uniqueID=\"18372017836343257010\"\>

          \<CMoveAnimTechIKPath removeNode=\"1\" /\>

        \</CMoveSingleAnim\>

      \</PerMoveResourceInfos\>

Almost same as replacing -- *CmoveSingleAnim* is selected by *uniqueID.*
Then its child *CMoveAnimTechIKPath* will be removed.

# Second part of combined - FCBData

As the name is saying, the second part is just classic fcb entries. Here
you can use same replace rules as in other FCB files like patch.dbt.fcb,
nomadobjecttemplates_rt.fcb, etc...

After you done a replace, put it to FCBData node:

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<DefaultInclude\>false\</DefaultInclude\>

  \<Name\>Combined Move File Sample Replace\</Name\>

\<Description\>\</Description\>

  

  \<Replaces\>

    \<Replace RequiredFile=\"move\\combinedmovefile.bin\"\>

      \<FCBData\>

        \...your replace here\...

      \</FCBData\>

    \</Replace\>

  \</Replaces\>

\</PackageInfoReplace\>

You can of course use both parts at the same time:

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Games\>

\<Game\>FC5\</Game\>

\</Games\>

  \<DefaultInclude\>false\</DefaultInclude\>

  \<Name\>Combined Move File Sample Replace\</Name\>

\<Description\>\</Description\>

  

  \<Replaces\>

    \<Replace RequiredFile=\"move\\combinedmovefile.bin\"\>

      \<PerMoveResourceInfos\>

        \<CMove_BlendRoot_DTRoot hash=\"BF4260DED58FDD4A\"\>

          \<CMoveTransition\>

            \<CMoveTransitionContainer count=\"1\"\>

              \<CMoveBlendRef layerParamOffset=\"our_new_value\" /\>

            \</CMoveTransitionContainer\>

          \</CMoveTransition\>

        \</CMove_BlendRoot_DTRoot\>

      \</PerMoveResourceInfos\>

      \<FCBData\>

        \...your replace here\...

      \</FCBData\>

    \</Replace\>

  \</Replaces\>

\</PackageInfoReplace\>
