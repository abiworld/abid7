(function ($) {
  Drupal.behaviors.bmj_video_player = {
    jQuery.fn.exists = function() {
      return jQuery(this).length > 0;
    },
    attach: function(context, settings) {
      $(document).ready(function() {
        /*
         *  Video page and pane jQuery custom function
         */
        if ($("a.avideoplayer").exists()) {

          $f("a.avideoplayer", "http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer-3.2.7.swf", {

            plugins: {
              rtmp: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.rtmp-3.2.3.swf'
              },
              gatracker: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.analytics-3.2.2.swf',
                events: {
                  all: true
                },
                //debug: true,
                accountId: "UA-432960-5"
              },

              viral: {
                // load the viral videos plugin
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.viralvideos-3.2.5.swf'
              }
            },


            clip: {
              provider: 'rtmp',
              eventCategory: 'Video'
            }

          })
        }

        /* Article based video */
        if ($("a.vplayer").exists()) {

          $f("a.vplayer", "http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer-3.2.7.swf", {

            plugins: {
              rtmp: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.rtmp-3.2.3.swf'
              },
              gatracker: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.analytics-3.2.2.swf',
                events: {
                  all: true
                },
                //debug: true,
                accountId: "UA-432960-5"
              },

              viral: {
                // load the viral videos plugin
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.viralvideos-3.2.5.swf'
              }
            },


            clip: {
              provider: 'rtmp',
              eventCategory: 'Video'
            }

          })
        }

      /*
       *  Video Slideshow / Playlist jQuery custom function
       */

        if ($("#playlistplayer").exists()) {

          var podcast_location = Drupal.settings.bmj_podcast.podcast_location;
          var podcast_filename = Drupal.settings.bmj_podcast.podcast_filename;
          var podcast_image = Drupal.settings.bmj_podcast.podcast_image;
          var podcast_title = Drupal.settings.bmj_podcast.podcast_title;

          $f("playlistplayer", "http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer-3.2.7.swf", {

            plugins: {
              rtmp: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.rtmp-3.2.3.swf'
                //durationFunc : 'getStreamLength',
                //netConnectionUrl : 'rtmp://fms.1efd.edgecastcdn.net/001EFD/video'
              },

              gatracker: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.analytics-3.2.2.swf',
                events: {
                  all: true
                },
                //debug: true,
                accountId: "UA-432960-5"
              },

              viral: {
                // load the viral videos plugin
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.viralvideos-3.2.5.swf'
              }
            },

            clip: {
              provider: 'rtmp',
              eventCategory: 'Video'
              //autoBuffering: true,
              //autoPlay: false,
            }

          }).playlist("div#playlist", {
            loop: true
          });

        }

        if ($("#playlistplayer-two").exists()) {


          //fix to make mp3 length work remove RTMP location
          $('.pitems').each(function(i) {
            $(this).attr('href', $(this).attr('href').replace(/rtmp:\/\/fms\.1efd\.edgecastcdn\.net\/001EFD\/bmjgroup\//i, ""));
          });
          var audio_title = '';
          $f("playlistplayer-two", "http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer-3.2.7.swf", {

            plugins: {
              rtmp: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.rtmp-3.2.3.swf',
                netConnectionUrl: 'rtmp://fms.1efd.edgecastcdn.net/001EFD/bmjgroup/',
                durationFunc: 'getStreamLength'
              },

              gatracker: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.analytics-3.2.2.swf',
                events: {
                  all: true
                },
                //debug: true,
                accountId: "UA-432960-5"
              },

              viral: {
                // load the viral videos plugin
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.viralvideos-3.2.5.swf'
              },
              controls: {
                autoHide: 'never'
              },
              // content plugin settings
              content: {
                url: 'flowplayer.content-3.2.0.swf',
                backgroundColor: '#002200',
                top: 25,
                right: 25,
                width: 260,
                height: 60,
                html: '<h2>Testing</h2>'
              }
            },

            // canvas background
            canvas: {
              backgroundImage: 'url(/sites/default/files/imagecache/bmj_podcast_widget_image/Sunset_0.jpg)'
            },
            clip: {
              provider: 'rtmp',
              eventCategory: 'Video',
              autoPlay: true
            }

          }).playlist("div#playlist2", {
            loop: true
          });
          //pitems

          $(".chapters").hide();
          $(".field-item").hide();
          $(".pitems").siblings(".podcast-chapters").hide();

          $('#playlist2 .pitems').click(function() {


            //    console.log($(this).siblings(''));

            audio_title = audio_title + $(this).text();

            $(this).siblings(".chapters").toggle();
            $(this).siblings(".field-item").toggle();
            $(this).siblings(".podcast-chapters").toggle();

            if ($f('playlistplayer-two').toggle()) {
              $f('playlistplayer-two').resume();
            } else {
              $f('playlistplayer-two').play();
            }
          });


          $('#playlist2 .pitems').click(function() {


            if ($f('playlistplayer-two').toggle()) {
              $f('playlistplayer-two').resume();
            } else {
              $f('playlistplayer-two').play();
            }
          });

          $(".podcast-chapters").click(function() {

            pminutes = $(this).children().filter(".pod-minute").text();
            pseconds = $(this).children().filter(".pod-seconds").text();
            tseconds = 60 * (parseInt(pminutes)) + parseInt(pseconds);

            $f("playlistplayer-two").seek(tseconds).resume();

            return false
          });


        }


        if ($("a.m-videoplayer").exists()) {

          $f("a.m-videoplayer", "http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer-3.2.7.swf", {

            plugins: {
              rtmp: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.rtmp-3.2.3.swf'
              },
              gatracker: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.analytics-3.2.2.swf',
                events: {
                  all: true
                },
                accountId: "UA-432960-5"
              }
      /*
              viral: {
                url: 'http://eso-cdn.group.bmj.com/media/flowplayer/flowplayer/flowplayer.viralvideos-3.2.5.swf'
              }
      */
            },


            clip: {
              provider: 'rtmp',
              eventCategory: 'Video'
            }

          })
        }
      });
    }
  }
})(jQuery);
