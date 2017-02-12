function addCommas(nStr){
    nStr += "";
    x = nStr.split(".");
    x1 = x[0];
    x2 = x.length > 1 ? "." + x[1] : "";
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, "$1" + "," + "$2");
    }
    return x1 + x2;
}
function calculateGrandTotal($){
    var grandTotal = 0;
    $('.assignment-value').each(function(){
        if ($(this).val()){
            grandTotal = grandTotal + parseFloat($(this).val());
        }
    });
    return grandTotal;
}
jQuery(function ($) {
    $('.input-assignment').on('click', function (e){
        e.preventDefault();
        var splitID = $(this).attr('id').split('-');
        window.location = Drupal.settings.basePath + 'assignmentzone/'+ splitID[1] +'/0/'+ $('#edit-month').val() +'/' + $('#edit-year').val();
    });
    $('.assignment-value').on('keyup', function(){
        var strID = $(this).attr('id');
        var splitID = strID.split('-');
        var totalValue = 0;
        if (Drupal.settings.jenisAssignment.periode == 0){
            var columnIdentity = splitID[5];
        }else{
            var columnIdentity = splitID[3];
        }
        $('.assignment-column-'+ columnIdentity).each(function(){
            if ($(this).val()){
                totalValue = totalValue + parseFloat($(this).val());
            }
        });
        $('#foot-'+ columnIdentity).html('<strong>'+ addCommas(totalValue) + '</strong>');
        var grandTotal = calculateGrandTotal($);
        $('#grand-total').html('<strong>GRAND TOTAL : '+ addCommas(grandTotal) +'</strong>');
    });
    var grandTotal = 0;
    $('.assignment-value').each(function(){
        if ($(this).val()){
            grandTotal = grandTotal + parseFloat($(this).val());
        }
    });
    $('#grand-total').html('<strong>GRAND TOTAL : '+ addCommas(grandTotal) +'</strong>');
    $('.input-offset').on('click', function(){
        var splitID = $(this).attr('id').split('-');
        if (typeof splitID[1] != 'undefined'){
            window.location = Drupal.settings.basePath + 'assignmentzone/'+ Drupal.settings.assigmentId +'/' + splitID[1] +'/'+ Drupal.settings.bulan +'/' + Drupal.settings.tahun;
        }
    });
})
