(function ($) {

/**
 * Loads media browsers and callbacks, specifically for media as a field.
 */
Drupal.behaviors.mediaInsert = {
  attach: function (context, settings) {
    // Options set from media.fields.inc for the types, etc to show in the browser.
    
    // For each widget (in case of multi-entry)
    $('.media-widget', context).once('mediaInsertButton', function () {
      
      $('<a class="media-insert" href="#">' + Drupal.t('Insert') + '</a>')
        .click(mediaInsertClick)
        .appendTo(this);
      
    });
  }
};

function mediaInsertClick() {

  var widget = $(this).parent('.media-widget');
  var fid = widget.find('.fid').val();
  var insertDialog = $('<div class="media-field"><input type="hidden" class="fid" value="' + fid + '" /><div><label for="media-caption">Caption: </label><input type="text" size="60" class="caption" id="media-caption" /></div><div class="media-field"><label for="media-alignment">Alignment:</label> <select class="align" id="media-alignment"><option value="left">Left</option><option value="right">Right</option><option value="center">Center</option></select></div></div>');
  var styles = Drupal.settings.media_insert_image_styles;
  var stylesList = $('<select class="image-style" id="media-style"></select>');
  var imageStyleDiv = $('<div class="media-field"><label for="media-style">Image Style:</label> </div>');
  var i;

  for (i in styles) {
    $('<option value="' + styles[i] + '">' + styles[i] + '</option>').appendTo(stylesList);
  } // for


  imageStyleDiv.append(stylesList);  
  imageStyleDiv.appendTo(insertDialog);

  if (fid > 0) {
  
    insertDialog.dialog({
      'autoOpen': true,
      'title': 'Insert Media',
      'modal': true,
      'position': 'center',
      'resizable': false,
      'width': 500,
      'closeOnEscape': true,
      'open': function(event, ui) { $(".ui-dialog-titlebar-close").hide(); },
      'buttons': {
        'Insert': mediaInsertInsert
      }
    });

  } // if

  return false;

} // mediaInsertClick

function mediaInsertInsert() {

  var body = $('#edit-body textarea.text-full');
  var content = body.val();
  var fid = $('.ui-dialog-content input.fid').val();
  var caption = $('.ui-dialog-content input.caption').val();
  var align = $('.ui-dialog-content select.align').val();
  var imageStyle = $('.ui-dialog-content select.image-style').val();

  body.val(content + '[[{"fid":' + fid + ',"caption":"' + caption + '","align":"' + align + '","image-style":"' + imageStyle + '"}]]');

  $('.ui-dialog-content').dialog('close');

} // mediaInsertInsert

})(jQuery);
