<script type="text/javascript">
jQuery(document).ready(function() {
	jQuery('.mapdot').hide();
        jQuery('.mapdot').fadeIn(1000);
});

</script>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
<?php

$xml_str = simplexml_load_file('http://www.rhok.org/map.xml');
//var_dump($xml_str);
//krumo($xml_str);
$i=0;


$node = node_load($nid);
$object = entity_metadata_wrapper('node', $node);
$body = $object->body->value();
print $body['value'];
?>
<div id="map">
<?php
foreach( $xml_str as $key => $value){
//	print_r($value);
/*
	print "<hr>";
	print "<h3>".$value->title."</h3>";
	print "<img src='".$value->field_logo."'>";
	print "<h3>".$value->title."</h3>";
	print "<p>".$value->field_citystate."</p>";
	print "<p>X: ".$value->field_map_x."</p>";
	print "<p>Y: ".$value->field_map_y."</p>";
	$i++;
	*/
	print "<div style='position:absolute; margin-left:".$value->x."px; margin-top:".$value->y."px;' class='mapdot'><a href='".$value->path."' class='maptooltip' title='".$value->title."'></a></div>";
}
?>

<img src="/sites/all/themes/rhoktober/images/rhok-map.gif" border="0" class="mapgif">
</div>
</div>
