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
* Declare information about a widget.
*
* @return
*   An array keyed by widget name. Each element of the array is an associative
*   array with these keys and values:
*   - "label": The human-readable label for the widget.
*   - "field types": An array of field type names that can be edited using
*     this widget.
*/
function cckpaypal_widget_info() {
  return array(
    'button' => array(
      'label' => 'Free donation button',
      'field types' => array('paypal_field'),
    ),
    'text' => array(
      'label' => 'Donate Button',
      'field types' => array('paypal_field'),
     ),
  );
}



/**
* Handle the parameters for a widget.
*
* @param $op
*   The operation to be performed. Possible values:
*   - "form": Display the widget settings form.
*   - "validate": Check the widget settings form for errors.
*   - "save": Declare which pieces of information to save back to the database.
* @param $widget
*   The widget on which the operation is to be performed.
* @return
*   This varies depending on the operation.
*   - The "form" operation should return an array of form elements to add to
*     the settings page.
*   - The "validate" operation has no return value. Use form_set_error().
*   - The "save" operation should return an array of names of form elements to
*     be saved in the database.
*/
function cckpaypal_widget_settings($op, $widget) {
  switch ($op) {
    case 'form':
      $form = array();
      $form['rows'] = array(
        '#type' => 'textfield',
        '#title' => t('Rows'),
        '#default_value' => $widget['rows'] ? $widget['rows'] : 1,
        '#required' => TRUE,
      );
      return $form;

    case 'validate':
      if (!is_numeric($widget['rows']) || intval($widget['rows']) != $widget['rows'] || $widget['rows'] <= 0) {
        form_set_error('rows', t('"Rows" must be a positive integer.'));
      }
      break;

    case 'save':
      return array('rows');
  }
}