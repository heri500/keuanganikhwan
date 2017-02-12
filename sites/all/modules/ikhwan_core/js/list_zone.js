jQuery(function ($) {
    $('.edit-zone').on('click', function (e){
        e.preventDefault();
        var splitID = $(this).attr('id').split('-');
        if (splitID.length > 0 && typeof splitID[2] != 'undefined'){
            window.location = Drupal.settings.basePath + 'masterdata/zone/add/'+ splitID[2];
        }
    });
    $('#new-zone').on('click', function (e){
        e.preventDefault();
        window.location = Drupal.settings.basePath + 'masterdata/zone/add';
    });
})
