function edit_premis(idPremis){
    window.location = Drupal.settings.basePath + 'masterdata/premis/add/'+ idPremis;
}
function manage_staff(idPremis){
    window.location = Drupal.settings.basePath + 'masterdata/premis/staff/'+ idPremis;
}
function overhead_premis(idPremis){
    window.location = Drupal.settings.basePath + 'masterdata/overhead_staff/'+ idPremis;
}
function delete_premis(idPremis){
    var konfirmasi = confirm('Yakin ingin menghapus data premis terkait...??!');
    if (konfirmasi){
        window.location = Drupal.settings.basePath + 'masterdata/premis/delete/'+ idPremis;
    }
}
jQuery(function ($) {
    $('#new-premis').on('click', function (e){
        e.preventDefault();
        window.location = Drupal.settings.basePath + 'masterdata/premis/add';
    });
})
