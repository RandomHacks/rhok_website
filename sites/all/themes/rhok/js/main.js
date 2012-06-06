(function ($) {
  
  $(document).ready(function(){
    
    /*  HOME
        - Latest from the RHoK Community  */
    if( $("#block-views-updates-block").length > 0 ){
      scrollUpdates();
    }
    
    /*  HOME
        - Partners  */
    if( $("#block-views-partners-block").length > 0 ){
      scrollPartners();
    }
    
    if($(".node-problem").length > 0) {
      condenseFlags($(".node-problem .link-wrapper"));
    }
    
    if($("#problem-node-form").length > 0) {
      setupProblemForm();
    }
  });

  /*----------------------------------------------------------------------------------------------------
  
  Scroll Updates
  
  ----------------------------------------------------------------------------------------------------*/
  
  var updatesScrollFactor = 3;
  var updatesScrolling = false;
  var updatesScrollIndex = 0;
  
  function scrollUpdates() {
    var content = $("#block-views-updates-block").find(".view-content");
    content.wrapInner("<div class='view-scrolling'></div>");
    var previous = $("<a href='#' class='previous'>Previous</a>").click(function(){
      switchUpdates(-1);
      return false;
    });
    var next = $("<a href='#' class='next'>Next</a>").click(function(){
      switchUpdates(1);
      return false;
    });
    content.before(previous, next);
    switchUpdates(0, true);
  }
  
  function switchUpdates(dir, force) {
    var newUpdatesScrollIndex = Math.max(0, updatesScrollIndex + dir*updatesScrollFactor);
    if(newUpdatesScrollIndex >= $("#block-views-updates-block").find(".view-scrolling").find(".views-row").length) {
      newUpdatesScrollIndex -= updatesScrollFactor;
    }
    if((!updatesScrolling && newUpdatesScrollIndex != updatesScrollIndex) || force){
      updatesScrolling = true;
      updatesScrollIndex = newUpdatesScrollIndex;
      if(updatesScrollIndex + updatesScrollFactor >= $("#block-views-updates-block").find(".view-scrolling").find(".views-row").length) {
        $("#block-views-updates-block").find(".next").addClass("disabled");
      } else {
        $("#block-views-updates-block").find(".next").removeClass("disabled");
      }
      if(updatesScrollIndex == 0){
        $("#block-views-updates-block").find(".previous").addClass("disabled");
      } else {
        $("#block-views-updates-block").find(".previous").removeClass("disabled");
      }
      
      var t = -($("#block-views-updates-block").find(".view-scrolling").find(".views-row").eq(updatesScrollIndex).position().top);
      
      //  Slide the Updates
      $("#block-views-updates-block").find(".view-scrolling").animate({
        top: t
      },500, function(){ updatesScrolling = false; })
      
      //  Fade Out Updates
      $("#block-views-updates-block").find(".view-scrolling").find(".views-row").each(function(ind){
        var hidden = .5;
        if($.browser.msie) {
          if($.browser.version < 8){
            hidden = 0;
          }
        }
        var a = (ind >= updatesScrollIndex && ind < updatesScrollIndex + updatesScrollFactor) ? 1 : hidden;
        $(this).animate({
          opacity: a
        }, 500);
      })
    }
  }
  
  /*----------------------------------------------------------------------------------------------------

  Scroll Partners

  ----------------------------------------------------------------------------------------------------*/
  
  var partnersScrollFactor = 6;
  var partnersScrolling = false;
  var partnersScrollIndex = 0;
  var partnersCounter = 0;
  
  function scrollPartners() {
    var content = $("#block-views-partners-block").find(".view-content");
        content.wrapInner("<div class='view-scrolling'></div>");
        $("#block-views-partners-block").find(".view-scrolling h3").wrap("<div class='scroll-section'></div>");
        $("#block-views-partners-block").find(".view-scrolling").children().wrap("<div class='scroll-item'></div>");
    var previous = $("<a href='#' class='previous'>Previous</a>").click(function(){
      switchPartners(-1);
      return false;
    });
    var next = $("<a href='#' class='next'>Next</a>").click(function(){
      switchPartners(1);
      return false;
    });
    content.before(previous, next);
    switchPartners(0, true);
    
    setInterval(checkPartnersCounter, 1000);
    
  }

  function switchPartners(dir, force) {
    partnersCounter = 0;
    var newPartnersScrollIndex = Math.max(0, partnersScrollIndex + dir*partnersScrollFactor);
    if(newPartnersScrollIndex >= $("#block-views-partners-block").find(".view-scrolling").children().length) {
      newPartnersScrollIndex -= partnersScrollFactor;
    }
    if((!partnersScrolling && newPartnersScrollIndex != partnersScrollIndex) || force){
      partnersScrolling = true;
      partnersScrollIndex = newPartnersScrollIndex;
      if(partnersScrollIndex + partnersScrollFactor >= $("#block-views-partners-block").find(".view-scrolling").children().length) {
        $("#block-views-partners-block").find(".next").addClass("disabled");
      } else {
        $("#block-views-partners-block").find(".next").removeClass("disabled");
      }
      if(partnersScrollIndex == 0){
        $("#block-views-partners-block").find(".previous").addClass("disabled");
      } else {
        $("#block-views-partners-block").find(".previous").removeClass("disabled");
      }

      var l = -($("#block-views-partners-block").find(".view-scrolling").children().eq(partnersScrollIndex).position().left);

      //  Slide the Partners
      $("#block-views-partners-block").find(".view-scrolling").animate({
        left: l
      },500, function(){ partnersScrolling = false; })

      //  Fade Out Partners
      $("#block-views-partners-block").find(".view-scrolling").children().each(function(ind){
        var a = (ind >= partnersScrollIndex && ind < partnersScrollIndex + partnersScrollFactor) ? 1 : .5;
        $(this).animate({
          opacity: a
        }, 500);
      })
    }
  }
  
  function checkPartnersCounter() {
    partnersCounter += 1;
    if(partnersCounter >= 6){
      partnersCounter = 0;
      var newPartnersScrollIndex = Math.max(0, partnersScrollIndex + partnersScrollFactor);
      if(newPartnersScrollIndex >= $("#block-views-partners-block").find(".view-scrolling").children().length) {
        newPartnersScrollIndex = 0;
      }
      partnersScrollIndex = newPartnersScrollIndex;
      switchPartners(0,true);
    }
  }
  
  /*----------------------------------------------------------------------------------------------------
  
  Condense Flags
  
  ----------------------------------------------------------------------------------------------------*/
  
  function condenseFlags(t){
    t.addClass("condensed");
    $("<a href='#' class='flag-this'>Flag This</a>").click(function(){
      t.find("ul").toggleClass("shown");
      $(this).toggleClass("shown");
      return false;
    }).prependTo(t);
  }
  
  /*----------------------------------------------------------------------------------------------------
  
  Problem Form
  
  ----------------------------------------------------------------------------------------------------
  
  function setupProblemForm() {
    var f = $("#problem-node-form");
    var fs = f.children("div").children("fieldset");
    
    var tabs = $("<div id='tabbed-form'></div>");
    f.prepend(tabs);
    
    var links = $("<ul class='tab-links'></ul>");
        links.appendTo(tabs);
        
    fs.each(function(index){
      $(this).appendTo(tabs);
      
      var id = $(this).attr("id");
      
      var legend = $(this).find("legend");
          legend.find(".form-required").remove();
      
      var li = $("<li><a href='#" + id + "'>" + legend.text() + "</a></li>");
      li.appendTo(links);
      
      legend.before($("<h2 class='form-title'>" + legend.text() + "</h2>"));
      legend.remove();
      
    })
    
    
      var myTabs = $("#tabbed-form").tabs();
      $('.ui-tabs-nav').addClass("bottomTabs");
      var x = $('.ui-tabs-nav').clone().removeClass("bottomTabs").addClass('topTabs');
      $('#tabbed-form').append(x);
      $('.topTabs').tabs();

      $('#tabbed-form').tabs({
          show: function(event, ui){
            window.location.hash = "#" + $(".ui-tabs-panel:not(.ui-tabs-hide)").attr("id");
          },
          select: function(event, ui) 
          {  
              index = ui.index;
              x = $('.topTabs').children('li:nth-child('+(parseInt(index)+1)+')');
              $('.topTabs').children('li').removeClass('ui-tabs-selected').removeClass('ui-state-active');
              x.addClass('ui-tabs-selected').addClass('ui-state-active');
          }
      });

      $('.topTabs').children('li').click(function () 
      {
          $('#tabbed-form').tabs("select", $('.topTabs').children('li').index(this));
      });
    
    
    $("#tabbed-form").removeClass("ui-corner-all ui-widget");
    $("#tabbed-form").find("").removeClass("ui-corner-all");
    $( "#tabbed-form .ui-tabs-nav, #tabbed-form .ui-tabs-nav > *" )
    			.removeClass( "ui-corner-all ui-corner-top" );
    
    $( "#tabbed-form .bottomTabs").wrap("<div class='form-steps-bottom'></div>");
    $("#edit-actions").appendTo($(".form-steps-bottom"));
    
    $( "#tabbed-form .topTabs").wrap("<div class='form-steps-top'></div>");
    $("#edit-actions").clone().appendTo($(".form-steps-top"));
    
    //$(".form-steps").clone().addClass("top-steps").prependTo(tabs);
    
    f.find(".vertical-tabs").css({
      "display":"none",
      "visibility":"hidden"
    });
  }
  */
})(jQuery); //$