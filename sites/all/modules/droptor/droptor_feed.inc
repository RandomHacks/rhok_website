<?php


/**
 * @file
 * The Droptor bridge module  
 * Droptor.com - Drupal site organization, monitoring, management and best practices
 * 
 * For more information, see:
 * - http://www.droptor.com
 * - http://drupal.org/project/droptor
 */

/**
 * Main feed wrapper routine, which calls all of the functions that gather data for returning to Droptor
 *
 * @param n/a
 *
 * @return
 *   A great big serialized set of data that Droptor improts
 */
function droptor_feed() {
	
  /*
   We only show the feed if the hash key is passed in and if the request comes from droptor.com
   (headers can be spoofed, so we use the hash keys too). It's not paranoia if they're really after you.
   */
  cache_clear_all('droptor', 'cache');

  // get hash, domain
  $correct_hash = droptor_check_hash();
  $correct_host = droptor_check_domain();
  $correct_security = droptor_check_connection_security();

  if ( $correct_hash && $correct_host && $correct_security ) {
    $data = array();
    $data['droptor-data-feed']['droptor_feed_version'] = '180';
    $data['droptor-data-feed']['monitor_text_match'] = variable_get('droptor_monitor_text_match', NULL);
    $data['droptor-data-feed']['ssl_login'] = droptor_ssl_login();
    $data['droptor-data-feed']['ssl_enabled'] = droptor_ssl_enabled();
    $data['droptor-data-feed']['settings_permissions_read_only'] = droptor_settings_permissions_read_only();
    $data['droptor-data-feed']['extra_files_removed'] = droptor_extra_files_removed();
    $data['droptor-data-feed']['node_statistics'] = droptor_node_statistics();
    $data['droptor-data-feed']['user_statistics'] = droptor_user_statistics();
    $data['droptor-data-feed']['statistics'] = droptor_statistics();
    $data['droptor-data-feed']['last_nodes'] = droptor_last_nodes();
    $data['droptor-data-feed']['last_users'] = droptor_last_users();
    $data['droptor-data-feed']['watchdog_size'] = droptor_watchdog_size();
    $data['droptor-data-feed']['site_name'] = variable_get('site_name', '{unknown}');
    $data['droptor-data-feed']['modules'] = droptor_modules();
    $data['droptor-data-feed']['variables_data'] = droptor_variables_data();
    $data['droptor-data-feed']['recent_pages_not_found'] = droptor_recent_pages_not_found();
    $data['droptor-data-feed']['last_logins'] = droptor_last_logins();
    $data['droptor-data-feed']['feed_over_ssl'] = variable_get('droptor_ssl_feed', 0);
    $data['droptor-data-feed']['memory_monitoring_status'] = droptor_memory_monitoring_status();
    $data['droptor-data-feed']['memory_monitoring_logs'] = droptor_memory_monitoring_logs();
    $data['droptor-data-feed']['user_1_name'] = droptor_user_1_name();
    $data['droptor-data-feed']['high_permission_count'] = droptor_high_permission_count();
    $data['droptor-data-feed']['module_update_data'] = droptor_module_update_data();
    $data['droptor-data-feed']['trends'] = droptor_get_trends();

    if (variable_get('droptor_debug_mode', 0) == 1) {
      $data['droptor-data-feed']['debug_remote_addr'] = ip_address();
    }

  	echo serialize($data);
  }
  elseif (!$correct_hash) {
    echo t('Invalid hash');
  }
  elseif (!$correct_host) {
    echo t('Invalid host') . '  (' . ip_address() . ')';
  }
  elseif (!$correct_security) {
    echo t('Invalid security');
  }

  exit; // http://stackoverflow.com/questions/2102688/how-do-you-replace-the-page-tpl-php-that-loads-from-a-module-in-drupal
}

/**
 * The total number of users and nodes created in the last hour, for proactive alerts
 *
 * @param n/a
 *
 * @return
 *   A serialized array of the total new users and nodes in the last hour
 */
function droptor_get_trends() {
   $total_users_created_last_hour = 0;
   $total_nodes_created_last_hour = 0;
   
   // get the total user count for the last hour
   $user_query = 'SELECT count(*) as usercount FROM {users} WHERE :time - 60*60 < created ORDER BY created DESC';
   $user_results = db_query($user_query, array('time'=>time()));
   foreach ($user_results as $user_result) {
      $total_users_created_last_hour = $user_result->usercount;
      break;
   }
   
   // get the total node count for the last hour
   $node_query = 'SELECT count(*) as nodecount FROM {node} WHERE :time - 60*60 < created ORDER BY created DESC';
   $node_results = db_query($node_query, array('time'=>time()));
   foreach ($node_results as $node_result) {
      $total_nodes_created_last_hour = $node_result->nodecount;
      break;
   }
   
   return array('new_users'=>$total_users_created_last_hour, 'new_nodes'=>$total_nodes_created_last_hour);
}

/**
 * Get the full module update status data for all modules
 *
 * @param n/a
 *
 * @return
 *   A serialized dump of the module update array data
 */
function droptor_module_update_data() {
  $module_meta_data = update_get_available(TRUE);
  return serialize(update_calculate_project_data($module_meta_data));
}

/**
 * The number of users who have admin user role.
 *
 * @param n/a
 *
 * @return
 *   A number
 */
function droptor_high_permission_count() {
   $count = null;
   $sql = 'SELECT COUNT(DISTINCT u.uid) as count FROM {users} u JOIN {users_roles} ur ON u.uid = ur.uid JOIN {role_permission} rp ON ur.rid = rp.rid WHERE u.status = 1 AND rp.module = \'user\' AND (rp.permission LIKE \'%administer permissions%\' OR rp.permission LIKE \'%administer users%\')';
   $results = db_query($sql);
   
   foreach ($results as $result) {
      $count = $result->count;      
      break;
   }
   
   return is_numeric($count) ? $count : null;
}

/**
 * The username of the Drupal super-duper user
 *
 * @param n/a
 *
 * @return
 *   The username field for user id = 1
 */
function droptor_user_1_name() {
   $return = null;
   $sql = 'SELECT name from {users} WHERE uid = 1 LIMIT 1';
   $results = db_query($sql);
   foreach ($results as $result) {
      $return = $result->name;
      break;
   }
   
   return $return;
}

/**
 * Memory monitoring has three states: on, off or not supported
 *
 * @param n/a
 *
 * @return
 *   The status of memory monitoring: 1, 0 or unsupported
 */
function droptor_memory_monitoring_status() {
	return !droptor_has_memory_management() ? 'unsupported' : variable_get('droptor_memory_monitoring',0);
}

/**
 * Pulls the memory usage logs from the database and cleans out the old ones
 *
 * @param n/a
 *
 * @return
 *   An array of memory log data or FALSE if turned off
 */
function droptor_memory_monitoring_logs() {
   $logs = false;
   if(variable_get('droptor_memory_monitoring',0) == 1) {
		$logs = array();
		// pull it out
		$query = 'SELECT q, dt, memory, execution_time FROM {droptor_memory} ORDER BY dt';
  		$results = db_query($query);
		foreach ($results as $result) {
		   $logs[] = array('q'=>$result->q,'dt'=>$result->dt,'memory'=>$result->memory,'execution_time'=>$result->execution_time);  			
		}
		// delete the data now that we have it
		db_delete('droptor_memory')->execute();	
	}
	
	return $logs;
}

/**
 * Grabs the failed logins in the last hour
 *
 * @param n/a
 *
 * @return
 *   An array of who and when of failed logins
 */
function droptor_last_logins() {
  $results = db_query('SELECT message, variables, timestamp FROM {watchdog} WHERE message LIKE :message AND :time - :lasthour < timestamp ORDER BY timestamp DESC', array(':message'=>'login attempt failed%',':lasthour' => 60 * 60,':time'=>time()));
  $last_logins = array();

  foreach ($results as $result) {
    $variables = unserialize($result->variables);
    $last_logins['failed'][$result->timestamp] = $variables['%user'];
  }
  return $last_logins;
}

/**
 * Grabs all system variables
 *
 * @param n/a
 *
 * @return
 *   An array of all of the variables for the site
 */
function droptor_variables_data() {
  $variables = array();
  $results = db_query('SELECT name, value FROM {variable} ORDER BY name');

  foreach ($results as $result) {
    $data = @unserialize($result->value);
    $variables[$result->name] = $data ? $data : NULL;
  }
  return serialize($variables);
}

/**
 * Grabs the last 404 errors in logs, excluding the checks we run for drupal files like README
 *
 * @param n/a
 *
 * @return
 *   An array of the pages not found and some associated data
 */
function droptor_recent_pages_not_found() {
  $data = array();
  $row = 0;

  $results = db_query_range('SELECT message, hostname, referer, timestamp FROM {watchdog} WHERE type = :type AND message NOT IN(:list) ORDER BY timestamp DESC', 
	0, 10, array(':type'=>'page not found',':list'=>'"UPGRADE.txt", "MAINTAINERS.txt", "README.txt", "INSTALL.pgsql.txt", "INSTALL.txt", "LICENSE.txt", "INSTALL.mysql.txt", "COPYRIGHT.txt", "CHANGELOG.txt"'));

  foreach ($results as $result) {
    $data[$row]['message'] = $result->message;
    $data[$row]['hostname'] = $result->hostname;
    $data[$row]['referer'] = $result->referer;
    $data[$row]['timestamp'] = $result->timestamp;

    $row++;
  }

  return $data;
}

/**
 * Grabs all system variables
 *
 * @param n/a
 *
 * @return
 *   An array of all of the variables for the site
 */
function droptor_watchdog_size() {
  return db_query('SELECT COUNT(*) FROM {watchdog}')->fetchField();
}

/**
 * Get total nodes created by day, for the last 30 days, for charting
 *
 * @param n/a
 *
 * @return
 *   An array of total nodes created each day for the last month
 */
function droptor_node_statistics() {
  $results = db_query_range("SELECT COUNT(*) AS total, DATE_FORMAT(FROM_UNIXTIME(created), '%m/%%d/%Y') AS dt FROM {node}  GROUP BY DATE_FORMAT(FROM_UNIXTIME(created), '%m/%%d/%Y') ORDER BY created DESC", 0, 30);
  $data = array();
  foreach ($results as $result) {
    $data[$result->dt] = $result->total;
  }

  return $data;
}

/**
 * Get total users created by day, for the last 30 days, for charting
 *
 * @param n/a
 *
 * @return
 *   An array of total users created each day for the last month
 */
function droptor_user_statistics() {
  $results = db_query_range("SELECT COUNT(*) AS total, DATE_FORMAT(FROM_UNIXTIME(created), '%m/%%d/%Y') AS dt FROM {users}  GROUP BY DATE_FORMAT(FROM_UNIXTIME(created), '%m/%%d/%Y') ORDER BY created DESC", 0, 30);
  $data = array();
  foreach ($results as $result) {
    $data[$result->dt] = $result->total;
  }

  return $data;
}

/**
 * Get lsat 15 nodes created
 *
 * @param n/a
 *
 * @return
 *   An array of the details of last 15 nodes created
 */
function droptor_last_nodes() {
  $last_five_nodes = array();
  $query = db_query_range('SELECT title, nid, created FROM {node} ORDER BY created DESC', 0, 15);

  $count = 0;
  foreach ($query as $data) {
    $last_five_nodes[$count]['url'] = drupal_get_path_alias('node/' . $data->nid);
    $last_five_nodes[$count]['title'] = $data->title;
    $last_five_nodes[$count]['created'] = $data->created;
    $count++;
  }

  return $last_five_nodes;
}

/**
 * Get lsat 15 users created
 *
 * @param n/a
 *
 * @return
 *   An array of the details of last 15 users created
 */
function droptor_last_users() {
  $last_five_users = array();
  $query = db_query_range('SELECT uid, mail, name, created FROM {users} ORDER BY created DESC', 0, 15);

  $count = 0;
  foreach ($query as $data) {
    $last_five_users[$count]['uid'] = $data->uid;
    $last_five_users[$count]['name'] = $data->name;
    $last_five_users[$count]['email'] = $data->mail;
    $last_five_users[$count]['created'] = $data->created;
    $count++;
  }

  return $last_five_users;
}

/**
 * Wrapper function to get node and user stats
 *
 * @param n/a
 *
 * @return
 *   An array of various node and user statistics
 */
function droptor_statistics() {
	return droptor_node_and_user_totals();
}

/**
 * A list of every module in the system
 *
 * @param n/a
 *
 * @return
 *   A serialized list of module details array
 */
function droptor_modules() {
  $query_result = db_query('SELECT name, status, info, type FROM {system} ORDER BY status DESC, name');
  $modules = array();

  foreach ($query_result as $data) {
    $info = unserialize($data->info);
    
    $modules[$data->name]['version'] = isset($info['version']) ? $info['version'] : NULL;
    $modules[$data->name]['status'] = $data->status;
    $modules[$data->name]['name'] = isset($info['name']) ? $info['name'] : $data->name;
    $modules[$data->name]['type'] = $data->type;
  }

  return serialize($modules);
}

/**
 * Check to see if the unneeded release files with Drupal are removed
 *
 * @param n/a
 *
 * @return
 *   True if they are removed, false if they aren't
 */
function droptor_extra_files_removed() {
  $clean_codebase = TRUE;
  $url = url(NULL, array('absolute' => TRUE));
  $files_to_remove = array('CHANGELOG.txt', 'COPYRIGHT.txt', 'INSTALL.mysql.txt', 'INSTALL.pgsql.txt', 'INSTALL.txt', 'LICENSE.txt', 
    'MAINTAINERS.txt', 'README.txt', 'UPGRADE.txt');

  foreach ($files_to_remove as $file) {
    $path = $_SERVER{'DOCUMENT_ROOT'} . base_path() . $file;
    if (file_exists($path))
      $clean_codebase = FALSE;
  }

  return $clean_codebase ? 1 : 0;
}

/**
 * Determines if settings.php is read-only
 *
 * @param n/a
 *
 * @return
 *   TRUE if the settings file is read-only, and FALSE if it isn't.
 */
function droptor_settings_permissions_read_only() {
  $settings_permissions_read_only = TRUE;
  $writes = array('2', '3', '6', '7'); // http://en.wikipedia.org/wiki/File_system_permissions
  $settings_file = './' . conf_path(FALSE, TRUE) . '/settings.php';
  $permissions = drupal_substr(sprintf('%o', fileperms($settings_file)), -4);

  foreach ($writes as $bit) {
    if (strpos($permissions, $bit)) {
      $settings_permissions_read_only = FALSE;
      break;
    }
  }

  return $settings_permissions_read_only;

}

/**
 * Checks to see if SSL login is required
 *
 * @param n/a
 *
 * @return
 *   True if login is secure, false if not
 */
function droptor_ssl_login() {
  $login_safe = 0;
  $securepages_login_query = db_query('SELECT value FROM {variable}  WHERE name = :name', array(':name' => 'securepages_pages'));
  foreach ($securepages_login_query as $securepages_login_results) {
    $pages = $securepages_login_results->value;
    if (strpos($pages, 'user*') > 0) {
      $login_safe = 1;
    } // to do - check the ignore pages field      
  }

  return $login_safe;
}

/**
 * Gathers various user and node stats for various time ranges
 * @todo - Total stats revamp in feed version 2
 *
 * @param n/a
 *
 * @return
 *   An array of data for various activity data over various recent time frames
 */
function droptor_node_and_user_time_stats() {
  $thirty_days_ago_in_seconds = REQUEST_TIME -(30 * 24 * 60 * 60);
  $seven_days_ago_in_seconds = REQUEST_TIME -(7 * 24 * 60 * 60);
  $twenty_four_hours_ago_in_seconds = REQUEST_TIME -(24 * 60 * 60);

  $stats = array();
  $stats['nodes_in_the_last_30_days'] = 0;
  $stats['nodes_in_the_last_7_days'] = 0;
  $stats['nodes_in_the_last_24_hours'] = 0;
  $stats['users_in_the_last_30_days'] = 0;
  $stats['users_in_the_last_7_days'] = 0;
  $stats['users_in_the_last_24_hours'] = 0;

  $nodes_result = db_query('SELECT created FROM {node}  WHERE created > :created', array(':created' => $thirty_days_ago_in_seconds));

  foreach ($nodes_result as $node) {
    if ($node->created > $twenty_four_hours_ago_in_seconds) {
      $stats['nodes_in_the_last_24_hours']++;
    }

    if ($node->created > $seven_days_ago_in_seconds) {
      $stats['nodes_in_the_last_7_days']++;
    }

    if ($node->created > $thirty_days_ago_in_seconds) {
      $stats['nodes_in_the_last_30_days']++;
    }
  }

  $users_result = db_query('SELECT created FROM {users}  WHERE created > :created', array(':created' => $thirty_days_ago_in_seconds));

  foreach ($users_result as $user) {
    if ($user->created > $twenty_four_hours_ago_in_seconds) {
      $stats['users_in_the_last_24_hours']++;
    }

    if ($user->created > $seven_days_ago_in_seconds) {
      $stats['users_in_the_last_7_days']++;
    }

    if ($user->created > $thirty_days_ago_in_seconds) {
      $stats['users_in_the_last_30_days']++;
    }
  }

  return $stats;
}

/* INTERNALS */
/* INTERNALS */
/* INTERNALS */

/**
 * Quickly check if a URL exists
 * b/c drupal_http_request() is wicked slow compared to straight curl
 *
 * @param n/a
 *
 * @return
 *   True if the URL exists, false otherwise
 */
function droptor_fast_url_check($url) {
  $valid = FALSE;

  if (in_array('curl', get_loaded_extensions())) {
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, $url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($curl, CURLOPT_HEADER, FALSE);
    curl_exec($curl);
    $result = curl_getinfo($curl);
    curl_close($curl);
    $valid = $result['http_code'] == '200' ? TRUE : FALSE;
  }
  else {
    $result = drupal_http_request($url);
    $valid = $result->code == 200 ? TRUE : FALSE;
  }

  return $valid;
}

/**
 * Total number of nodes (and by type) and total user count
 *
 * @param n/a
 *
 * @return
 *   An array of user and node stats
 */
function droptor_node_and_user_totals() {
  $stats = array();

  // total users
  $total_users = db_query('SELECT COUNT(*) as total_users FROM {users}')->fetchField();
  $stats = is_numeric($total_users) ? array_merge($stats, array('total_users'=>$total_users)) : $stats;

  // total nodes
  $total_nodes_query = db_query('SELECT COUNT(*) as total_nodes, nt.name as type FROM {node} n JOIN {node_type} nt ON n.type = nt.type GROUP BY type ORDER BY nt.name');
  $total_nodes = 0;
  $nodes_by_type = array();
  
  foreach ($total_nodes_query as $node_type_data) {
    $nodes_by_type[$node_type_data->type] = $node_type_data->total_nodes;
    $total_nodes += $node_type_data->total_nodes;
  }

  $stats['total_nodes'] = $total_nodes;
  $stats['total_nodes_by_type'] = $nodes_by_type;

  return $stats;
}