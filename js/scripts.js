/*-------------SHAMELESSLY COPIED-----------------*/
/*--------stackoverflow.com---------------------*/
var jump=function(e)
{
    //alert('here');
   if (e){
       //e.preventDefault();
       var target = jQuery(this).attr("href").replace('/', '');
   }else{
       var target = location.hash;
   }

   jQuery('html,body').animate(
   {
       scrollTop: (jQuery(target).offset().top) - 100
   },500,function()
   {
       //location.hash = target;
   });

}

jQuery('html, body').hide();

jQuery(document).ready(function($)
{
    $(document).on('click', 'a[href*=#]', jump);

    if (location.hash){
        setTimeout(function(){
            $('html, body').scrollTop(0).show();
            jump();
        }, 0);
    }else{
        $('html, body').show();
    }
});
