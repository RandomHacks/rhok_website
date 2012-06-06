<?php

/**
 * @file
 * Lightweight implementation of the Twitter API in PHP.
 *
 * This code does the heavy lifting behind the Drupal twitter_block module. It
 * does not aim to authenticate users nor provide complex integration. We only 
 * need to grab public feeds, and as such we use the Twitter Search API. For 
 * more information on the twitter search API, @see 
 * @link http://dev.twitter.com/doc/get/search
 */

/**
 * TwitterSearch provides the class for using the Twitter Search API.
 *
 * For mor information on the API, see http://dev.twitter.com/doc/get/search
 */
class TwitterSearch {

  // HTTP status code returned
  private $http_status;

  // Search parameters as defined by the API to be used w/ http_build_query
  private $options = array();

  // Determines which getter to use.
  private $search_type;

  // What were we looking for again?
  private $search_string;
  private $twitter_name;

  // The url including the encoded query
  public $url_query;

  public function __construct($config = array()) {
    $this->search_type = $config['search_type'];
    
    if ( $config['search_type'] == 'searchHashtag' ) {
      // We presume the search string is already validated. 
      $this->search_string = $config['search_string'];
    }
    else {
      $this->twitter_name = $config['search_string'];
    }

    // The number of tweets to return per page.
    if ( !empty( $config['results_per_page']) ) {
      $this->options['rpp'] = $config['results_per_page'];
    }
    else {
      $this->options['rpp'] = variable_get('twitter_block_default_results_per_page', 15);
    }

    // Filter by language, but only if there is one set in the config.
    if (isset($config['lang']) && !empty($config['lang'])) {
      $this->options['lang'] = $config['lang'];
    }
  }

  /**
   * Retrieve JSON encoded search results
   */
  public function getJSON() {
    return call_user_func(array($this, $this->search_type)); 
  }

  /**
   * Returns the most recent tweets from $twittername 
   * @param string $twittername to search. Note: begins with @
   * @return string $json JSON encoded search response
   */
  private function getTweetsFrom() {
    $this->options['q'] = "from:$this->twitter_name";
    $json = $this->search();
    return $json;
  }

  /**
   * Returns the most recent mentions (status containing @twittername)
   * @param string $twittername to search. Note: begins with @
   * @return string $json JSON encoded search response
   */
  private function getMentions() {
    $this->options['q'] = "@$this->twitter_name";
    $json = $this->search();
    return $json;
  }

  /**
   * Returns the most recent @replies to $twittername.
   * @param string $twittername to search. Note: begins with @. 
   * @return string JSON encoded search response
   */
  private function getReplies() {
    $this->options['q'] = "to:$this->twitter_name";
    $json = $this->search();
    return $json;
  }

  /**
   * Returns the most recent tweets containing a string or hashtag.
   * @param string $hashtag to search. May or may not begin with #. 
   * @return string JSON encoded search response
   */
  private function searchHashtag() {
    $this->options['q'] = ($this->search_string);
    $json = $this->search();
    return $json;
  }

  /**
   * Returns the last HTTP status code
   * @return integer
   */
  public function lastStatusCode() {
    return $this->http_status;
  }

  /**
   * Executes a Twitter Search API call
   * @return string JSON encoded search response.
   */
  function search() {
    $this->url_query = 'http://search.twitter.com/search.json?' . http_build_query($this->options);
    $ch = curl_init($this->url_query);
    
    // Applications must have a meaningful and unique User Agent. 
    curl_setopt($ch, CURLOPT_USERAGENT, "Drupal Twitter Block Module");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Expect:'));

    $twitter_data = curl_exec($ch);
    $this->http_status = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    curl_close($ch);

    return $twitter_data;
  }
}
?>
