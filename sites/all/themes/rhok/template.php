<?php
/*
function rhok_preprocess_page{$variables, $hook){

if (arg(0) == 'user') {
    switch (arg(1)) {
      case 'register':
        $vars['title'] = t('Create a new account');
        break;

      case 'password':
        $vars['title'] = t('Retrieve lost password');
        break;

      case '':
      case 'login':
        $vars['title'] = t('User login');
        break;

    }
  }
}
*/

/**
 * Returns themed html for individual tweets
 */
function rhok_twitter_block_tweets($tweet_object, $variables = array() ) {
  $tweet = get_object_vars($tweet_object['tweet']);
  $tweet['text'] = twitter_block_linkify($tweet['text']);
  $tweet['time_ago'] = format_interval(time() - strtotime($tweet['created_at']));
  $html = <<<EOHTML
<div class="twitter_block tweet">
  <div class="tweet_text"><p class="tweet">{$tweet['text']}</p></div> 
  <div class="twitter_block_user">
    <a class="twitter_block profile_image" href="http://twitter.com/{$tweet['from_user']}">
      <span class="twitter_block_user_name">@{$tweet['from_user']}</span>
    </a>
    <span class="twitter_block_time_ago">{$tweet['time_ago']}</span>
  </div>
</div>
EOHTML;
  
  return $html;
}

/**
 * Implements theme_file_styles_image().
 */
function rhok_file_styles_image($variables) {

  $json = $variables['instance']->object->override['json'];

  $output = '<div class="media-image format-' . drupal_html_class($json['image-style']) . ' align-' . $json['align'] . '">';

  if (isset($json['image-style'])) {
    $output .= '<div class="image">' . theme('image_style', array('style_name' => $json['image-style'], 'path' => $variables['image_uri'], 'alt' => $variables['alt'], 'title' => $variables['title'], 'getsize' => FALSE, 'attributes' => $variables['attributes'])) . '</div>';
  }
  else {
    $output .= '<div class="image">' . theme('image', array('path' => $variables['image_uri'], 'alt' => $variables['alt'], 'title' => $variables['title'], 'getsize' => FALSE, 'attributes' => $variables['attributes'])) . '</div>';
  }

  if ($json['caption']) {
    $output .= '<div class="caption">' . check_plain($json['caption']) . '</div>';
  } // if

  $output .= '</div>';

  return $output;

} // rhok_file_styles_image

?>
