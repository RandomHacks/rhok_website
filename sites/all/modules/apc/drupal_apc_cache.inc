<?php

/**
 * @file
 * This integrates the drupal APC cache backend.
 */
$apc_statistics = array();

/**
 * APC cache implementation.
 *
 * This is Drupal's APC cache implementation. It uses Alternative PHP
 * Cache to store cached data. Each cache bin corresponds to a prefix of
 * the apc variables with the same name.
 */
class DrupalAPCCache implements DrupalCacheInterface {
  protected $bin;

  function __construct($bin) {
    $this->bin = $bin;
  }

  /**
   * Function which retrieves the safe key for the cache bin.
   *
   * @return
   *   The safe APC key.
   */
  private function binKey() {
    static $binKey = array();

    $test_info = &$GLOBALS['drupal_test_info'];
    $bin = (!empty($test_info['test_run_id']) ? ('_' . $test_info['test_run_id']) : '') . $this->bin;

    if (!isset($binKey[$bin])) {
      $binKey[$bin] = $_SERVER['HTTP_HOST'] . '|' . $bin . '|';
    }
    return $binKey[$bin];
  }

  /**
   * Function which retrieves the safe key for the cache cid.
   *
   * @param $cid
   *   The cache id.
   * @return
   *   The safe APC key.
   */
  private function key($cid) {
    return $this->binKey() . $cid;
  }

  function get($cid) {
    // Garbage collection necessary when enforcing a minimum cache lifetime.
    $this->garbageCollection($this->bin);

    // Add a get to our statistics.
    $GLOBALS['apc_statistics'][] = array('get', $this->bin, array($cid));

    // Fetch the data.
    $cache = apc_fetch($this->key($cid));
    return $this->prepareItem($cache);
  }

  /**
   * Prepare a cached item.
   *
   * Checks that items are either permanent or did not expire, and unserializes
   * data as appropriate.
   *
   * @param $cache
   *   An item loaded from cache_get() or cache_get_multiple().
   * @return
   *   The item with data unserialized as appropriate or FALSE if there is no
   *   valid item to load.
   */
  protected function prepareItem($cache) {
    global $user;

    if (!isset($cache->data)) {
      return FALSE;
    }
    // If the data is permanent or we are not enforcing a minimum cache lifetime
    // always return the cached data.
    if ($cache->expire == CACHE_PERMANENT || !variable_get('cache_lifetime', 0)) {
      if ($cache->serialized) {
        $cache->data = unserialize($cache->data);
      }
    }
    // If enforcing a minimum cache lifetime, validate that the data is
    // currently valid for this user before we return it by making sure the cache
    // entry was created before the timestamp in the current session's cache
    // timer. The cache variable is loaded into the $user object by _drupal_session_read()
    // in session.inc. If the data is permanent or we're not enforcing a minimum
    // cache lifetime always return the cached data.
    if ($cache->expire != CACHE_PERMANENT && variable_get('cache_lifetime', 0) && (isset($user->cache) && $user->cache > $cache->created)) {
      // This cache data is too old and thus not valid for us, ignore it.
      return FALSE;
    }

    if (isset($cache->headers)) {
      $cache->headers = unserialize($cache->headers);
    }

    return $cache;
  }

  function getMultiple(&$cids) {
    // Garbage collection necessary when enforcing a minimum cache lifetime.
    $this->garbageCollection($this->bin);

    // We need to search the cache with the proper keys and
    // be able to get the original $cid back.
    foreach ($cids as $cid) {
      $keys[$this->key($cid)] = $cid;
    }

    $fetch = apc_fetch(array_keys($keys));
    $cache = array();
    foreach ($fetch as $key => $data) {
      $cache[$keys[$key]] = $this->prepareItem($fetch[$key]);
    }
    unset($fetch);

    // Add a get to our statistics.
    $GLOBALS['apc_statistics'][] = array('get', $this->bin, $cids);

    $cids = array_diff($cids, array_keys($cache));

    return $cache;
  }

  /**
   * Garbage collection for get() and getMultiple().
   *
   * @param $bin
   *   The bin being requested.
   */
  protected function garbageCollection() {
    global $user;

    // Garbage collection necessary when enforcing a minimum cache lifetime.
    $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
    if ($cache_flush && ($cache_flush + variable_get('cache_lifetime', 0) <= REQUEST_TIME)) {
      // Reset the variable immediately to prevent a meltdown in heavy load situations.
      variable_set('cache_flush_' . $this->bin, 0);
      // Time to flush old cache data
      $this->clearTemporary();
    }
  }

  function set($cid, $data, $expire = CACHE_PERMANENT, array $headers = NULL) {
    // Add set to statistics.
    $GLOBALS['apc_statistics'][] = array('set', $this->bin, $cid);

    // Create new cache object.
    $cache = new stdClass();
    $cache->cid = $cid;
    $cache->serialized = 0;
    $cache->created = REQUEST_TIME;
    $cache->expire = $expire;
    $cache->headers = isset($headers) ? serialize($headers) : NULL;

    if (is_object($data)) {
      $cache->serialized = 1;
      $cache->data = serialize($data);
    }
    else {
      $cache->data = $data;
    }

    // What kind of expiration is being used.
    switch ($expire) {
      case CACHE_PERMANENT:
        $set_result = apc_store($this->key($cid), $cache);
        break;

      case CACHE_TEMPORARY:
        if (variable_get('cache_lifetime', 0) > 0) {
          $set_result = apc_store($this->key($cid), $cache, variable_get('cache_lifetime', 0));
        }
        else {
          $set_result = apc_store($this->key($cid), $cache);
        }
        break;

      default:
        $set_result = apc_store($this->key($cid), $cache, $expire - time());
        break;
    }
  }

  function clearTemporary() {
    $data = apc_cache_info('user');
    $count = count($data['cache_list']);

    for ($i = 0; $i < $count; $i++) {
      if (strpos($data['cache_list'][$i]['info'], $this->binKey()) === 0) {
        $cache = apc_fetch($data['cache_list'][$i]['info']);
        if ($cache && $cache->expire == CACHE_TEMPORARY) {
          apc_delete($data['cache_list'][$i]['info']);
        }
      }
    }
  }

  function clear($cid = NULL, $wildcard = FALSE) {
    global $user;

    // Add a get to our statistics.
    $GLOBALS['apc_statistics'][] = array('clear', $this->bin, $cid, (int)$wildcard);

    if (empty($cid)) {
      if (variable_get('cache_lifetime', 0)) {
        // We store the time in the current user's $user->cache variable which
        // will be saved into the sessions bin by _drupal_session_write(). We then
        // simulate that the cache was flushed for this user by not returning
        // cached data that was cached before the timestamp.
        $user->cache = REQUEST_TIME;

        $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
        if ($cache_flush == 0) {
          // This is the first request to clear the cache, start a timer.
          variable_set('cache_flush_' . $this->bin, REQUEST_TIME);
        }
        elseif (REQUEST_TIME > ($cache_flush + variable_get('cache_lifetime', 0))) {
          // Clear the cache for everyone, cache_lifetime seconds have
          // passed since the first request to clear the cache.
          $this->clearTemporary();
          variable_set('cache_flush_' . $this->bin, 0);
        }
      }
      else {
        $this->clearTemporary();
      }
    }
    else {
      if ($wildcard) {
        $data = apc_cache_info('user');
        $count = count($data['cache_list']);

        if ($cid == '*') {
          for ($i = 0; $i < $count; $i++) {
            if (strpos($data['cache_list'][$i]['info'], $this->binKey()) === 0) {
              apc_delete($data['cache_list'][$i]['info']);
            }
          }
        }
        else {
          for ($i = 0; $i < $count; $i++) {
            if (strpos($data['cache_list'][$i]['info'], $this->key($cid)) === 0) {
              apc_delete($data['cache_list'][$i]['info']);
            }
          }
        }
      }
      elseif (is_array($cid)) {
        foreach ($cid as $delete_cid) {
          apc_delete($this->key($delete_cid));
        }
      }
      else {
        apc_delete($this->key($cid));
      }
    }
  }

  function isEmpty() {
    $data = apc_cache_info('user');
    $count = count($data['cache_list']);

    for ($i = 0; $i < $count; $i++) {
      if (strpos($data['cache_list'][$i]['info'], $this->binKey()) === 0) {
        return FALSE;
      }
    }

    return TRUE;
  }
}
