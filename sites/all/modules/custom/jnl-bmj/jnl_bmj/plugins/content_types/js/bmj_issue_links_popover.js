(function ($) {
  Drupal.behaviors.popoverIssueLinks = {
    attach: function(context, settings) {
      $('#issue-cover-plus').popover({
        trigger: 'click',
        placement: 'bottom',
        html: true,
        content: function() {
          return $('#issue-links-popover').html();
        }
      }).css('z-index', 1010);
    }
  };
})(jQuery);
