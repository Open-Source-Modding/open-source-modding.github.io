**How to use dice**

**This document covers new dice introduced in the Mod Installer version
20230401-1700.**

Dice is a special feature for randomizing numbers, for example for
config IDs. The number is randomized during every next installation of
packages.

Defining *Dice*, it is placed directly in info or info_replace:

\<Dice\>

\<Roll ID=\"Number1\" Min=\"0\" Max=\"9\" /\>

\<Roll ID=\"Number2\" Min=\"10\" Max=\"19\" /\>

\<Roll ID=\"Number3\" Min=\"20\" Max=\"29\" /\>

\<Roll ID=\"Number4\" Min=\"30\" Max=\"39\" /\>

\</Dice\>

\<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\"?\>

\<PackageInfoReplace\>

  \<Games\>

\<Game\>FC6\</Game\>

\</Games\>

  \<Name\>My Package\</Name\>

  \<Description\>My desc.\</Description\>

  

\<Dice\>

\<Roll ID=\"Number1\" Min=\"0\" Max=\"9\" /\>

\<Roll ID=\"Number2\" Min=\"10\" Max=\"19\" /\>

\<Roll ID=\"Number3\" Min=\"20\" Max=\"29\" /\>

\<Roll ID=\"Number4\" Min=\"30\" Max=\"39\" /\>

\</Dice\>

  \<Replaces\>

    \...

  \</Replaces\>

\</PackageInfoReplace\>

As you can see, there are four definitions of randomizing number with
ranges:

- 0 to 9

- 10 to 19

- 20 to 29

- 30 to 39

The *Min* number is inclusive and *Max* number is exclusive.

Note: if you're also using options, make sure your dice ID is not the
same with an option ID, both must be unique.

# Using Dice in Replace

For this you need to understand **How to use replace package**.

To use a randomized number, you simply add the dice ID to field value,
like this:

\<field hash=\"8EDB0295\" name=\"disLibItemId\"
type=\"Id64\"\>{Number1}\</field\>

This means that the field will get a randomized number from *Roll* with
*ID* **Number1**, so it will be a number from range 0 to 9.

# Using Dice in templateValue

For this you need to understand **How to use replace package** and **How
to use templates in info**.

You can even use the randomized number as param for *template*:

\<template id=\"MyTemplate\" templateValueID=\"{Number2}\" /\>

This means that *templateValue* with name **ID** will receive a
randomized number from *Roll* with *ID* **Number2**, so it will be a
number from range 10 to 19.

So without *Dice* it looks like:

\<template id=\"MyTemplate\" templateValueID=\"A Value\" /\>

and with *Dice* with *Roll ID* **Number2**:

\<template id=\"MyTemplate\" templateValueID=\"{Number2}\" /\>
