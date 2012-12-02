<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML+RDFa 1.0//EN"
  "http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language; ?>" version="XHTML+RDFa 1.0" dir="<?php print $language->dir; ?>"<?php print $rdf_namespaces; ?>>

<head profile="<?php print $grddl_profile; ?>">
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>
<script type="text/javascript">
jQuery(document).ready(function() {
jQuery('body.page-user h1.page-title').text('Log In');
jQuery('body.page-user-register h1.page-title').text('Create New Account');
jQuery('body.page-user-password h1.page-title').text('Request New Password');
jQuery('input#edit-mailchimp-lists-mailchimp-1-subscribe').attr('checked','checked');
});
</script>
</head>
<body class="<?php print $classes; ?>" <?php print $attributes;?>>
<?php
if(isset($jump_link_target)){
?>
  <div id="skip-link">
    <a href="#<?php print $jump_link_target; ?>" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
  </div>
<?php
}
?>
  <?php print $page_top; ?>
  <?php print $page; ?>
  <?php print $page_bottom; ?>
</body>
</html>
