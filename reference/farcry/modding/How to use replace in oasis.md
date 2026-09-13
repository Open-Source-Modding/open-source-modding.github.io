**How to use replace in oasis**

If you want to add or replace strings in oasis files, you can also use
info_replace.xml.

Here are some examples.

## Replacing a string

If you want to replace whole string, then you can do:

\<Replace RequiredFile=\"languages\\english\\oasisstrings.oasis.bin\"\>

  \<section name=\"0x00990284\"\>

    \<string enum=\"0x99977001\" id=\"100000\" value=\"My replaced string\" /\>

  \</section\>

\</Replace\>

First you need define which oasis file will be processed. Every language
oasis file must have own *Replace* tag.

If you want to replace *string* value, you need also define *section*
which contain your *string* tag.

## Adding new strings

If you want to add new strings, you can do it like this:

\<Replace RequiredFile=\"languages\\english\\oasisstrings.oasis.bin\"\>

  \<section addNode=\"1\" name=\"0x00990284\"\>

    \<string addNode=\"1\" enum=\"0x99977001\" id=\"100000\" value=\"My new string\" /\>

    \<string addNode=\"1\" enum=\"0x99977002\" id=\"100001\" value=\"My second new string\" /\>

  \</section\>

\</Replace\>

Every line which will be added must contain attribute *addNode* with
value 1.

Also make sure than *enum* and *id* values are unique in whole oasis
file.

## Replacing a word in whole oasis file

Replacing is case sensitive, so take care about it. Of course you define
*ReplaceString* as many as you want.

\<Replace RequiredFile=\"languages\\english\\oasisstrings.oasis.bin\"\>

  \<ReplaceString old=\"Boomer\" new=\"Timber\" /\>

\</Replace\>
