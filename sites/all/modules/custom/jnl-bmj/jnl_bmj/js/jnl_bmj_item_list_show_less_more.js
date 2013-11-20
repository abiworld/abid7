(function ($) {
  // Store our function as a property of Drupal.behaviors.
  Drupal.behaviors.bmj_more_less_links = {
    attach: function (context, settings) {
      $('a.bmj-more-less-link', context).once('bmj-more-less-link', function() {
        $(this).click(function(event) {
          var context_class = $(this).attr('data-context');
          $('.' +  context_class + ' .bmj-list-item-hide').toggleClass('bmj-hide-list-item');
          $(this).parents('.' + context_class).toggleClass('show-full');
          event.preventDefault();
        });
      });
    }
  };
}(jQuery));
