// $Id$;

DESCRIPTION 
-------------------------- 
The Organic Groups module (also referred to as the 'og' module), provides users 
the ability to create, manage, and delete their own 'groups' on a site.
Each group can have members, and maintains a group home page which individual 
group members may post into. Posts can be sent to multiple groups (i.e. cross-
posted), and individual posts (referred as 'group content') may be shared with 
members, or non-members where necessary.
Group membership can be open, closed or moderated.

TERMS AND DEFINITIONS
------------------------------------
- GROUP: A single node which can have different content types and users 
  associated with it.
- GROUP CONTENT: Content such as nodes or users, which are associated with a 
  group.
- GROUP ADMIN: Is a privileged user with permission to administer particular 
  activities within a group.
- SITE ADMIN: Compared to group admin, a site admin is granted access to all 
  groups operating within a site. The site admin can specify the permissions 
  group admins are granted in order to control their group related activities, 
  while keeping other permissions out of their reach.
- GROUP CONTEXT: Whenever an individual piece of content such as a node or a 
  user is viewed, the module attempts to determine if the content is associated 
  with a particular group.
  The group context is later on used to determine which access rights the user 
  is granted. For example, in a particular group context the user can edit 
  nodes, but is only allowed to view the nodes in a different group context.
  The group context can also be used by custom modules to determine different 
  behaviors. For example, displaying different blocks on different groups, 
  switching to a different theme, etc.
- ENTITY: Nodes, users, and taxonomy terms, are examples of Drupal entities. 
  Organic Groups allows each individual Drupal entity type to be associated with 
  a group or with a group content. This means that you can associate different 
  users (as group content) to a certain user (as a group).

GROUP ARCHITECTURE
--------------------------
At the lowest level the module associates content types with groups. Above this 
level is the role and permissions layer, which operates at the group level.
The Organic Groups module leverages Drupal's core functionality, especially the 
field API. This means that a content type is associated with a group, by setting 
the correct field value.
Users are also allowed to select the groups that will be associated with the 
content from a list of groups, which they have authorization to view. 
As is the case with Drupal itself, in Organic Groups different permissions can 
be assigned to different user roles. This allows group members to perform a 
different set of actions, in different group contexts.

INSTALLATION DRUPAL 7.x
--------------------------------------------
Note that the following guide is here to get you started. Names for content 
types, groups and group content given here are suggestions and are given to 
provide a quick way to get started with Organic groups.

1. Enable the Group and the Group UI modules.
2. Create a new content type via admin/structure/types/add. Call it "Group", and 
   define it to be of Group type.
3. Create a second content type. Call it "Group content", and set it to be of 
   Group content type.
4. Add a Group by going to node/add/group. Call it First group, and enable the 
   Group through the "Group type" field.
5. Add a Group Content by going to node/add/group-content. In the Groups 
   audience field, select First group. In the group content view a link was 
   added to the group.
6. Click on the Group link. In the group view, a new tab was added labeled 
   Group.
7. Click on the Group tab. You will be redirected to the group administration 
   area. Note that this is the administration of First group only. It will not 
   affect existing or new groups which will be created on the site.
8. You are now presented with different actions you can perform within the 
   group. Such as add group members, add roles, and set member permissions. You 
   will notice that these options have the same look and feel as Drupal core in 
   matters relating to management of roles and permissions.
9. Back in the group view you will also notice a 'Subscribe' link, shown to 
   privileged users. Subscribing is the act of associating a user with a group.
10. In order to associate other entities with group or group content, navigate 
    to Organic Groups field settings", in admin/config/og/fields. 
11. In order to define default permissions for groups that are newly created or 
    to edit permissions on all existing groups, navigate to the Group 
    default permissions page. Important permissions in this page are the ones 
    under the administer section. These permissions are what enable group admins 
    to have granular control over their own group. This means, that if you as 
    the site admin, don't want to allow group admins to control who can edit 
    nodes in their own group, you need to uncheck those permissions.

DEVELOPERS & SITE BUILDERS
----------------------------------------------
- You may craft your own URLs which produce useful behavior. For example, 
  node/add/group-content?gids_node[]=4 will add a select the group with node ID 
  4, in the node form. The prefixed entity can change to indicate other entity 
  types allowing crafting the URL and you can have multiple variables, for 
  example, node/add/group-content?gids_node[]=4&gids_user[]=3&gids_group[]=5,6,7
  The above URL will select the group with node ID 4, and the group with user ID
  3, and the groups with the unique group ID 5, 6 and 7.
  Note that the actaul entity of group ID 5, 6 and 7 can be any entity (e.g. 
  nodes or users).    


CREDITS
----------------------------
- Organic groups for Drupal 5 and 6 authored by Moshe Weitzman -  
  <weitzman AT tejasa DOT com>
- Current project maintainer and Drupal 7 author is Amitai Burstein (Amitaibu) - 
  gizra.com
