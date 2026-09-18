**How to use replace in RML format**

**This document covers new options introduced in the Mod Installer
version 20230401-1700.**

RML is format used in FC4 ndb (or in FC5 in cseq) files. Since RML can
contain complicated structures like FCB, you can use replacer also for
this format.

You need first to know basic replacer function so please look to „How to
use replace package" first.

Replacer for RML is almost same as the one for FCB, so I show here
examples „how it look in FCB and how it look in RML" + small whole
example.

# Differences

RML has all fields and names known, so output XML looks different than
FCB one.

# primaryKey

First big different is primaryKey. In FCB you define it as node, but in
RML you define it as attribute of a node you want to select.

+-----------------------------------------------------------------+-------------------------------------------------------------+
| ### FCB | ### RML |
+-----------------------------------------------------------------+-------------------------------------------------------------+
| \<object hash=\"BB96607D\"\> | \<ManagedObject primaryKey_disLibItemId=\"7667812319534\"\> |
| | |
| \<primaryKey hash=\"8EDB0295\"\>EA0A817C58072000\</primaryKey\> | \</ManagedObject\> |
| | |
| \</object\> | |
+-----------------------------------------------------------------+-------------------------------------------------------------+

> *Note: hash BB96607D is „ManagedObject", hash 8EDB0295 is
> „disLibItemId"*

primaryKey format in RML is like this: „primaryKey\_\<name of an
attribute\>", so from the example above it's *disLibItemId* -- it means
attribute with name disLibItemId will be selected as primary key and the
value of the disLibItemId attribute is searched.

# removeParent

Usage of this is same:

### FCB

\<object hash=\"BB96607D\"\>

\<primaryKey hash=\"8EDB0295\" removeParent=\"1\"\>EA0A817C58072000\</primaryKey\>

\</object\>

### RML

\<ManagedObject primaryKey_disLibItemId=\"7667812319534\" removeParent=\"1\"\>

\</ManagedObject\>

# addNode

Usage of this is same, but it can have two versions -- first is adding
new *object* and second is adding new *field*. Look below.

Adding new object:

### FCB

\<object hash=\"59F2984F\" name=\"Generic\"\>

  \<primaryKey hash=\"8EDB0295\" Name=\"disLibItemId\"\>7366C26342072000\</primaryKey\>

  \<object hash=\"452629B8\" name=\"InventoryItemData\" addNode=\"1\"\>

    \<field hash=\"298E6C00\" name=\"text_tmplWeaponPropertiesName\" type=\"String\"\>weaponproperties:fcx.autopistols.a-99\</field\>

  \</object\>

\</object\>

### RML

\<Generic primaryKey_disLibItemId=\"7667812330301\"\>

  \<InventoryItemData addNode=\"1\" text_tmplWeaponPropertiesName=\"weaponproperties:fcx.autopistols.a-99\"\>

  \</InventoryItemData\>

\</Generic\>

Here in adding new *object* it's pretty same.

Adding new field:

### FCB

\<object hash=\"59F2984F\" name=\"Generic\"\>

  \<primaryKey hash=\"8EDB0295\" Name=\"disLibItemId\"\>7366C26342072000\</primaryKey\>

  \<object hash=\"452629B8\" name=\"InventoryItemData\"\>

    \<field hash=\"298E6C00\" name=\"text_tmplWeaponPropertiesName\" type=\"String\" addNode=\"1\"\>weaponproperties:fcx.autopistols.a-99\</field\>

  \</object\>

\</object\>

### RML

\<Generic primaryKey_disLibItemId=\"7667812330301\"\>

  \<InventoryItemData text_tmplWeaponPropertiesName=\"weaponproperties:fcx.autopistols.a-99\"\>

  \</InventoryItemData\>

\</Generic\>

But here, do you see the difference? Yes, in RML you don't define
*addNode* for *field*. If you add any new attribute then it's
automatically added.

# clearNodeBeforeInsert

Usage of this is same:

### FCB

\<object hash=\"4004E0AD\" name=\"ShopTypeDatas\" clearNodeBeforeInsert=\"1\"\>

\</object\>

### RML

\<ShopTypeDatas clearNodeBeforeInsert=\"1\"\>

\</ShopTypeDatas\>

# makeParentDuplicate

Usage of this is same:

### FCB

\<object hash=\"BB96607D\" name=\"ManagedObject\"\>

  \<primaryKey hash=\"8EDB0295\" makeParentDuplicate=\"1\"\>7366C26342072000\</primaryKey\>

  \<field hash=\"8EDB0295\" name=\"disLibItemId\" type=\"Id64\"\>7667813675335\</field\>

  \<field hash=\"D91FFFD3\" name=\"disDbObjectName\" type=\"String\"\>LMGs/My_U100\</field\>

\</object\>

### RML

\<Generic primaryKey_disLibItemId=\"7667813675334\" makeParentDuplicate=\"1\" disLibItemId=\"7667813675335\" Name=\"LMGs/My_U100\"\>

\</Generic\>

# Templates

Here is a bigger difference. Since RML uses more atributes than nodes
then templates were changed a bit more.

First look how looks calling a template:

### FCB

\<object hash=\"1F027DA1\" name=\"NomadObject\"\>

  \<template id=\"Weapon\" templateValueID=\"7667812340759\" templateValueName=\"Assault rifles/My_P416\" templateValuePrice=\"175000\" /\>

\</object\>

### RML

\<lib\>

  \<ModInstallerTemplate id=\"Weapon\" modInstallerTemplateValueID=\"7667812340759\" modInstallerTemplateValueName=\"Assault rifles/My_P416\" modInstallerTemplateValuePrice=\"175000\" /\>

\</lib\>

In RML it is used *ModInstallerTemplate* rather than *template*. It's
due to that there can be already an attribute named as *template*, so
that's why it has different name. But in overal it's almost same.

Now look how looks template itself (example shortened):

### FCB

\<Templates\>

  \<template id=\"Weapon\"\>

    \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

      \<field hash=\"8EDB0295\" name=\"disLibItemId\" type=\"Id64\" templateValue=\"ID\" /\>

      \<field hash=\"D91FFFD3\" name=\"disDbObjectName\" type=\"String\" templateValue=\"Name\" /\>

      \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\"\>

        \<object hash=\"D481BBBF\" name=\"ShopTypeData\"\>

          \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\" templateValue=\"Price\"\>145000\</field\>

        \</object\>

      \</object\>

    \</object\>

  \</template\>

\</Templates\>

### RML

\<Templates\>

  \<template id=\"Weapon\"\>

    \<Generic disLibItemId=\"modInstallerTemplateValueID\" Name=\"modInstallerTemplateValueName\"\>

      \<ShopTypeDatas\>

        \<ShopTypeData buyValue=\"modInstallerTemplateValuePrice;145000\" /\>

      \</ShopTypeDatas\>

    \</Generic\>

  \</template\>

\</Templates\>

As you can see first two params (ID, Name) has no default value, but the
third one has default value 145000. In RML it must be defined like that
-- template value name and default value separated by „;".

If you set default value to „null", the attribute will not be added.

# Mod Installer package option

Usage of this is same:

### FCB

\<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\"\>{OptionID}\</field\>

### RML

\<ShopTypeData buyValue=\"{OptionID}\" /\>

# Now complete info_replace.xml example

This example is used for adding new weapon to the store + other changes
in store (change price for an item, Mod Installer Option, remove an
item). Please take a note that this is shortened example, so it will not
work in the game (the template doesn't contain full store config entry
but rather only those fields required for this example).

Also, take a note that examples are quite different because the FCB one
is created for FC5 and the RML one is for FC4.

> *Note: You can copy example to any editor for better reading.*

### FCB

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Replaces\>

    \<Replace RequiredFile=\"patch.dbt.fcb\"\>

      \<!\-- add new two items using template \--\>

      \<object hash=\"1F027DA1\" name=\"NomadObject\"\>

        \<template id=\"Weapon\" templateValueID=\"7667812340759\" templateValueName=\"Assault rifles/My_P416\" templateValuePrice=\"175000\" /\>

        \<template id=\"Weapon\" templateValueID=\"7667812340760\" templateValueName=\"Assault rifles/My_Sec_P416\" /\>

      \</object\>

      

      \<!\-- change price \--\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<primaryKey hash=\"8EDB0295\" name=\"disLibItemId\"\>7850613187731\</primaryKey\>

        \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\"\>

          \<object hash=\"D481BBBF\" name=\"ShopTypeData\"\>

            \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\"\>500100\</field\>

          \</object\>

        \</object\>

      \</object\>

      \<!\-- clear prices and new entries \--\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<primaryKey hash=\"8EDB0295\" name=\"disLibItemId\"\>7667812319534\</primaryKey\>

        \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\" clearNodeBeforeInsert=\"1\"\>

          \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\" addNode=\"1\"\>55120\</field\>

        \</object\>

      \</object\>

      \<!\-- same as above but using template \--\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<primaryKey hash=\"8EDB0295\" name=\"disLibItemId\"\>7667812330301\</primaryKey\>

        \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\" clearNodeBeforeInsert=\"1\"\>

          \<template id=\"Price\" templateValuePrice=\"119000\" /\>

        \</object\>

      \</object\>

      \<!\-- remove the item completely \--\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<primaryKey hash=\"8EDB0295\" name=\"disLibItemId\" removeParent=\"1\"\>7667814946874\</primaryKey\>

      \</object\>

      

      \<!\-- change price using Mod Installer Option \--\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<primaryKey hash=\"8EDB0295\" name=\"disLibItemId\"\>7667814962322\</primaryKey\>

        \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\"\>

          \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\" ModInstallerOption=\"1\" /\>

        \</object\>

      \</object\>

      

      \<!\-- make duplicate of the item and change some things \--\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<primaryKey hash=\"8EDB0295\" name=\"disLibItemId\" makeParentDuplicate=\"1\"\>7667813675334\</primaryKey\>

        \<field hash=\"8EDB0295\" name=\"disLibItemId\" type=\"Id64\"\>7667813675335\</field\>

        \<field hash=\"D91FFFD3\" name=\"disDbObjectName\" type=\"String\"\>LMGs/My_U100\</field\>

        \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\"\>

          \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\"\>199500\</field\>

        \</object\>

      \</object\>

    \</Replace\>

  \</Replaces\>

  \<Templates\>

    \<template id=\"Weapon\"\>

      \<object hash=\"BB96607D\" name=\"ManagedObject\"\>

        \<field hash=\"8EDB0295\" name=\"disLibItemId\" type=\"Id64\" templateValue=\"ID\" /\>

        \<field hash=\"D91FFFD3\" name=\"disDbObjectName\" type=\"String\" templateValue=\"Name\" /\>

        \<object hash=\"4004E0AD\" name=\"ShopTypeDatas\"\>

          \<object hash=\"D481BBBF\" name=\"ShopTypeData\"\>

            \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\" templateValue=\"Price\"\>145000\</field\>

          \</object\>

        \</object\>

      \</object\>

    \</template\>

    \<template id=\"Price\"\>

      \<field hash=\"7F055B07\" name=\"buyValue\" type=\"Int32\" templateValue=\"Price\" /\>

    \</template\>

  \</Templates\>

\</PackageInfoReplace\>

### RML

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Replaces\>

    \<Replace RequiredFile=\"generated\\databases\\generic\\shoppingitems.ndb\"\>

      \<!\-- add new two items using template \--\>

      \<lib\>

        \<ModInstallerTemplate id=\"Weapon\" modInstallerTemplateValueID=\"7667812340759\" modInstallerTemplateValueName=\"Assault rifles/My_P416\" modInstallerTemplateValuePrice=\"175000\" /\>

        \<ModInstallerTemplate id=\"Weapon\" modInstallerTemplateValueID=\"7667812340760\" modInstallerTemplateValueName=\"Assault rifles/My_Sec_P416\" /\>

      \</lib\>

      

      \<!\-- change price \--\>

      \<Generic primaryKey_disLibItemId=\"7850613187731\"\>

        \<ShopTypeDatas\>

          \<ShopTypeData primaryKey_shptypShopType=\"2\" buyValue=\"500100\" /\>

        \</ShopTypeDatas\>

      \</Generic\>

      \<!\-- clear prices and new entries \--\>

      \<Generic primaryKey_disLibItemId=\"7667812319534\"\>

        \<ShopTypeDatas clearNodeBeforeInsert=\"1\"\>

          \<ShopTypeData addNode=\"1\" shptypShopType=\"1\" buyValue=\"55120\" sellValue=\"-1\" bModulateCostOfItemWithKarmaLevel=\"1\" arkKarmaLevelPricesCurve=\"7866560178293\" /\>

          \<ShopTypeData addNode=\"1\" shptypShopType=\"2\" buyValue=\"55120\" sellValue=\"-1\" bModulateCostOfItemWithKarmaLevel=\"1\" arkKarmaLevelPricesCurve=\"7866560178293\" /\>

          \<ShopTypeData addNode=\"1\" shptypShopType=\"255\" buyValue=\"-1\" sellValue=\"-1\" bModulateCostOfItemWithKarmaLevel=\"0\" /\>

        \</ShopTypeDatas\>

      \</Generic\>

      \<!\-- same as above but using template \--\>

      \<Generic primaryKey_disLibItemId=\"7667812330301\"\>

        \<ShopTypeDatas clearNodeBeforeInsert=\"1\"\>

          \<ModInstallerTemplate id=\"Price\" modInstallerTemplateValuePrice=\"119000\" modInstallerTemplateValueKarmaModulate=\"1\" modInstallerTemplateValuePriceCurve=\"7866560178293\" /\>

        \</ShopTypeDatas\>

      \</Generic\>

      \<!\-- remove the item completely \--\>

      \<Generic primaryKey_disLibItemId=\"7667814946874\" removeParent=\"1\" /\>

      

      \<!\-- change price using Mod Installer Option \--\>

      \<Generic primaryKey_disLibItemId=\"7667814962322\"\>

        \<ShopTypeDatas\>

          \<ShopTypeData buyValue=\"ModInstallerOption\" /\>

        \</ShopTypeDatas\>

      \</Generic\>

      

      \<!\-- make duplicate of the item and change some things \--\>

      \<Generic primaryKey_disLibItemId=\"7667813675334\" makeParentDuplicate=\"1\" disLibItemId=\"7667813675335\" Name=\"LMGs/My_U100\"\>

        \<ShopTypeDatas\>

          \<ShopTypeData buyValue=\"199500\" /\>

        \</ShopTypeDatas\>

      \</Generic\>

    \</Replace\>

  \</Replaces\>

  \<Templates\>

    \<template id=\"Weapon\"\>

      \<Generic disLibItemId=\"modInstallerTemplateValueID\" Name=\"modInstallerTemplateValueName\"\>

        \<ShopTypeDatas\>

          \<ShopTypeData buyValue=\"modInstallerTemplateValuePrice;145000\" /\>

        \</ShopTypeDatas\>

      \</Generic\>

    \</template\>

    \<template id=\"Price\"\>

      \<ShopTypeData shptypShopType=\"1\" buyValue=\"modInstallerTemplateValuePrice;1\" sellValue=\"-1\" bModulateCostOfItemWithKarmaLevel=\"modInstallerTemplateValueKarmaModulate;0\" arkKarmaLevelPricesCurve=\"modInstallerTemplateValuePriceCurve;null\" /\>

      \<ShopTypeData shptypShopType=\"2\" buyValue=\"modInstallerTemplateValuePrice;1\" sellValue=\"-1\" bModulateCostOfItemWithKarmaLevel=\"modInstallerTemplateValueKarmaModulate;0\" arkKarmaLevelPricesCurve=\"modInstallerTemplateValuePriceCurve;null\" /\>

      \<ShopTypeData shptypShopType=\"255\" buyValue=\"-1\" sellValue=\"-1\" bModulateCostOfItemWithKarmaLevel=\"0\" /\>

    \</template\>

  \</Templates\>

\</PackageInfoReplace\>

So, this show RML replacer is different from the FCB one. As you can see
RML is more attribute based.

I hope you understand how to make a RML replacer now. As always, if you
want ask something, do not hesitate to ask me :D
