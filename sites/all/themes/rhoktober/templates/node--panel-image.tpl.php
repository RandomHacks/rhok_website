<article<?php print $attributes; ?>>
  <div<?php print $content_attributes; ?>>
    <?php
//	krumo($content);

//	print render($content);

if($content['field_panel_image'][0]['#item']['filename']){
	if($content['field_panel_link']['#items'][0]['url']){
        	$url = $content['field_panel_link']['#items'][0]['url'];
	}
	else{
        	$url == "#";
	}

	if($content['field_panel_image'][0]['#item']['alt']){
                $caption = $content['field_panel_image'][0]['#item']['alt'];
        }
        else{
                $caption == "";
        }


print "<a href='".$url."' title='".$caption."'><img src='/sites/default/files/panel_images/".$content['field_panel_image'][0]['#item']['filename']."' border='0' alt='".$caption."'></a>";
}
    ?>
  </div>
</article>

