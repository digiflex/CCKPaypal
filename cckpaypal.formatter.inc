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
* Declare information about a formatter.
*
* @return
*   An array keyed by formatter name. Each element of the array is an associative
*   array with these keys and values:
*   - "label": The human-readable label for the formatter.
*   - "field types": An array of field type names that can be displayed using
*     this formatter.
*/
function cckpaypal_field_formatter_info() {
  return array(
    'default' => array(
      'label' => 'Default',
      'field types' => array('text'),
    ),
    'plain' => array(
      'label' => 'Plain text',
      'field types' => array('text'),
    ),
    'trimmed' => array(
      'label' => 'Trimmed',
      'field types' => array('text'),
    ),
  );
}

/**
* Prepare an individual item for viewing in a browser.
*
* @param $field
*   The field the action is being performed on.
* @param $item
*   An array, keyed by column, of the data stored for this item in this field.
* @param $formatter
*   The name of the formatter being used to display the field.
* @param $node
*   The node object, for context. Will be NULL in some cases.
*   Warning : when displaying field retrieved by Views, $node will not
*   be a "full-fledged" node object, but an object containg the data returned
*   by the Views query (at least nid, vid, changed)
* @return
*   An HTML string containing the formatted item.
*
* In a multiple-value field scenario, this function will be called once per
* value currently stored in the field. This function is also used as the handler
* for viewing a field in a views.module tabular listing.
*
* It is important that this function at the minimum perform security
* transformations such as running check_plain() or check_markup().
*/
function cckpaypal_field_formatter($field, $item, $formatter, $node) {
  if (!isset($item['value'])) {
    return '';
  }
  if ($field['text_processing']) {
    $text = check_markup($item['value'], $item['format'], is_null($node) || isset($node->in_preview));
  }
  else {
    $text = check_plain($item['value']);
  }
  
  switch ($formatter) {
    case 'plain':
      return strip_tags($text);
    
    case 'trimmed':
      return node_teaser($text, $field['text_processing'] ? $item['format'] : NULL);
    
    default:
      return $text;
  }
}



