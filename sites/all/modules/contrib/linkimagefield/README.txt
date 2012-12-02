LinkImageField provides an "LinkImage" widget type to Drupal's Field API. This
module extends the ImageField widget, allowing a user to provide a URL link for
it.

Dependencies
------------
 * Field
 * Image

5.x Version was written by Kamal Challa
6.x Version was written by John Young
7.x Version was written by skilip

Install
-------

1) Copy the linkimagefield folder to the modules folder in your installation.

2) Enable the module using Administer -> Modules
   (/admin/modules).

3) Create a new linkimagefield through the Field interface. Visit Administer ->
   Structure -> Content types (admin/structure/types), then click
   Manage fields on the type you want to add an image upload field. Select
   "Link Image" as the field type and "Link Image" as the widget type to create a new
   field.
   
4) To establish the proper display, make sure that the correct formatter is 
   selected. Visit Administer -> Structure -> Content types 
   (admin/structure/types), then click Display fields on the type with the 
   Link Image Field.  Make sure "LinkImage" is selected for Teaser and Body.
