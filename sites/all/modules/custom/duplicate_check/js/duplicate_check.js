(function ($) {

  $(document).ready(function() {

    $('#edit-title').blur(function(){
    
      $('#similar-problems').remove();
    
      $('<div id="similar-problems" class="waiting"></div>').insertAfter('.form-item-title');

      $.getJSON(
        Drupal.settings.basePath + 'duplicate_check/search/' + encodeURIComponent($('#edit-title').val()), {}, 
        function(data, textStatus, jqXHR) {

          var prompt = Drupal.t('Please confirm this is not a duplicate of the existing problems');            
          var similar = $('<ul></ul>');

          $('#similar-problems').removeClass('waiting');

          if (data.length > 0) {

            $('<h2>' + prompt + '</h2>').appendTo('#similar-problems');
            
            for (var i in data) {
              if (i < 3) {
                similar.append('<li><h3><a href="' + data[i].link + '">' + data[i].title + '</a></h3></li>');
              } // if
            } // for
            
            similar.appendTo('#similar-problems');

            $('<a href="#" class="close">Close</a>')
              .click(function() {
              
                $('#similar-problems').remove();
                return false;
              
              })
              .appendTo('#similar-problems');
          
          } // if
        
        }
      );
    
    });

    /*
if ($('#edit-title').val() == '') {

      var prompt = Drupal.t('Before you post your problem definition, please enter a title search for similar problem definitions. One problem definition with two owners is better than two very similar ones.');
      var titleDialog = $('<div><p>' + prompt + '</p><div><input type="text" id="duplicate-check-title" class="form-text" size="40" /></div></div>');
  
      titleDialog.dialog({
        'autoOpen': true,
        'title': 'Problem Title',
        'modal': true,
        'position': 'center',
        'resizable': false,
        'width': 500,
        'closeOnEscape': false,
        'open': function(event, ui) { $(".ui-dialog-titlebar-close").hide(); },
        'buttons': {
          'Ok': duplicateCheckOk
        }
      });

    } // if
*/
  
  });


})(jQuery);