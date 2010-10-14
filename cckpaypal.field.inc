<?php
/**
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
* Declare information about a field type.
*
* @return
*   An array keyed by field type name. Each element of the array is an associative
*   array with these keys and values:
*   - "label": The human-readable label for the field type.
*/
function cckpaypal_field_info() {
  return array(
    'number_integer' => array('label' => t('Paypal Field')),
    'number_decimal' => array('label' => 'Decimal'),
  );
}
/**
* Handle the parameters for a field.
*
* @param $op
*   The operation to be performed. Possible values:
*   - "form": Display the field settings form.
*   - "validate": Check the field settings form for errors.
*   - "save": Declare which fields to save back to the database.
*   - "database columns": Declare the columns that content.module should create
*     and manage on behalf of the field. If the field module wishes to handle
*     its own database storage, this should be omitted.
*   - "filters": If content.module is managing the database storage,
*     this operator determines what filters are available to views.
*     They always apply to the first column listed in the "database columns"
*     array.
* @param $field
*   The field on which the operation is to be performed.
* @return
*   This varies depending on the operation.
*   - The "form" operation should return an array of form elements to add to
*     the settings page.
*   - The "validate" operation has no return value. Use form_set_error().
*   - The "save" operation should return an array of names of form elements to
*     be saved in the database.
*   - The "database columns" operation should return an array keyed by column
*     name, with arrays of column information as values. This column information
*     must include "type", the MySQL data type of the column, and may also
*     include a "sortable" parameter to indicate to views.module that the
*     column contains ordered information. Details of other information that can
*     be passed to the database layer can be found at content_db_add_column().
*   - The "filters" operation should return an array whose values are 'filters'
*     definitions as expected by views.module (see Views Documentation).
*     When providing several filters, it is recommended to use the 'name'
*     attribute in order to let the user distinguish between them. If no 'name'
*     is specified for a filter, the key of the filter will be used instead.
*/
function cckpaypal_field_settings($op, $field) {
  switch ($op) {
    case 'form':
      $form = array();
      $form['max_length'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum length'),
        '#default_value' => $field['max_length'] ? $field['max_length'] : '',
        '#required' => FALSE,
        '#description' => t('The maximum length of the field in characters. Leave blank for an unlimited size.'),
      );
      $form['allowed_values'] = array(
        '#type' => 'textarea',
        '#title' => t('Allowed values list'),
        '#default_value' => isset($field['allowed_values']) ? $field['allowed_values'] : '',
        '#required' => FALSE,
        '#rows' => 10,
        '#description' => t('The possible values this field can contain.'),
      );

      return $form;

    case 'save':
      return array('max_length', 'allowed_values');

    case 'database columns':
      $columns = array(
        'value' => array('type' => 'varchar', 'not null' => TRUE, 'default' => "''", 'sortable' => TRUE),
        'format' => array('type' => 'int', 'length' => 10, 'unsigned' => TRUE, 'not null' => TRUE, 'default' => 0),
      );
      if ($field['max_length'] == 0 || $field['max_length'] > 255) {
        $columns['value']['type'] = 'longtext';
      }
      else {
        $columns['value']['length'] = $field['max_length'];
      }
      return $columns;

    case 'filters':
      return array(
        'substring' => array(
          'operator' => 'views_handler_operator_like',
          'handler' => 'views_handler_filter_like',
        ),
        'alpha_order' => array(
          'name' => 'alphabetical order',
          'operator' => 'views_handler_operator_gtlt',
        ),
      );
  }
}

function cckpaypal_content_is_empty() {
	
}

/**
* Define the behavior of a field type.
*
* @param $op
*   What kind of action is being performed. Possible values:
*   - "load": The node is about to be loaded from the database. This hook
*     should be used to load the field.
*   - "view": The node is about to be presented to the user. The module
*     should prepare and return an HTML string containing a default
*     representation of the field.
*   - "validate": The user has just finished editing the node and is
*     trying to preview or submit it. This hook can be used to check or
*     even modify the node. Errors should be set with form_set_error().
*   - "submit": The user has just finished editing the node and the node has
*     passed validation. This hook can be used to modify the node.
*   - "insert": The node is being created (inserted in the database).
*   - "update": The node is being updated.
*   - "delete": The node is being deleted.
* @param &$node
*   The node the action is being performed on. This argument is passed by
*   reference for performance only; do not modify it.
* @param $field
*   The field the action is being performed on.
* @param &$items
* An array containing the values of the field in this node. Changes to this variable will
* be saved back to the node object.
* Note that, in order to ensure consistency, this variable contains an array regardless of
* whether field is set to accept multiple values or not.
* @return
*   This varies depending on the operation.
*   - The "load" operation should return an object containing extra values
*     to be merged into the node object.
*   - The "view" operation should return a string containing an HTML
*     representation of the field data.
*   - The "insert", "update", "delete", "validate", and "submit" operations
*     have no return value.
*
* In most cases, only "view" and "validate" are relevant operations; the rest
* have default implementations in content_field() that usually suffice.
*/
function cckpaypal_field($op, &$node, $field, &$items, $teaser, $page) {
   switch ($op) {
    case 'view':
      foreach ($items as $delta => $item) {
        $items[$delta]['view'] = content_format($field, $item, 'default', $node);
      }
      return theme('field', $node, $field, $items, $teaser, $page);

    case 'validate':
      $allowed_values = "ABC";// text_allowed_values($field);

      if (is_array($items)) {
        foreach ($items as $delta => $item) {
          $error_field = $field['field_name'].']['.$delta.'][value';
          if ($item['value'] != '') {
            if (count($allowed_values) && !array_key_exists($item['value'], $allowed_values)) {
              form_set_error($error_field, t('Illegal value for %name.', array('%name' => t($field['widget']['label']))));
            }
          }
        }
      }
      break;
  }
}