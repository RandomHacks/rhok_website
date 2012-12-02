
Scrollable Content provides a scrolling functionality for your content. Scrollable Content gives you a nice content slider preview for your featured site's content, and provides some display options (e.g. number of sliding contents, and speed of scrolling).

Scrollable Contnet ships with two main features:
1- Non-tech person who wants a quick and pretty slider for her website (read usage section to learn how to do that).
2- Drupaler who wants a full control over this module, can use Views, CCK, and Template to control and override most of this module's options.


This is a module developed by Khaled Al Hourani (Site: www.khaledalhourani.com) - (Email: info@holooli.com)

Scrollable Content uses jQuery library and the very nice plugin "jQuery Scrollable" (http://flowplayer.org/tools/scrollable.html).


Install:
------------

1- Non-tech way:
  1- @todo: how to download and upload this module
  2- Go to modules page (http://yoursite.com/admin/build/modules), look for Scrollable Content and enable it.
  3- Then change the default settings for this module to suit your needs (http://yoursite.com/admin/settings/scrollable_content).
  4- Finally go to Blocks page (http://yoursite.com/admin/build/block), look for Scrollable Content block and drag it to the region (e.g. Header) you want it to appear in.
  5- To display scrolling or sliding contents you got to have an image inside the node, you can either use IMCE (http://drupal.org/project/imce) to put images inside node's body and Scrollable Content will detect them, or a CCK filed (image field), and specify that field in Scrollable Content settings page (http://yoursite.com/admin/settings/scrollable_content) in "Images field name" select box.
  6- We are done! wait!! if you find any difficulty you can watch an instruction video (@todo: demo url).

2- Drupaler way:
  0- No need to say that you have to enable this module.
  1- If you want Scrollable Content to show only specific content, you can either assign a taxonomy term to the featured nodes (e.g. create a term called "featured"), and then heading back to Scrollable Content's settings page (admin/settings/scrollable_content) and select that term from "Only in the category" select box, or create a new content type and select it from "Only of the type" select box.
  2- Another option is to use Views, create a new view with all filters and other Views options you want, then specifiy the "Style" of this view as "Scrollable Content", click Update, and change Scrollable Content style options from the next form.
  3- Or use the basic feature to display featured content as block from Blocks page (see Non-tech guide above) .
  4- Additionally, you can theme Scrollable Content block or view by copying template files from Scrollable Content directory to your theme's directory and editing them (@todo: what is this? -> e.g. node-featured.tpl.php, node-ContentTypeName.tpl.php).
  5- For images size you can use Imagecache from Scrollable Content settings page. 


Contact:
-----------

Written by Khaled Al Hourani AKA good_man (Site: www.khaledalhourani.com) - (Email: info@holooli.com)

If you have a job for me (not freelancing but a job), drop me an email at info@holooli.com