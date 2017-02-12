function addCommas(nStr){
    nStr += "";
    x = nStr.split(".");
    x1 = x[0];
    x2 = x.length > 1 ? "." + x[1] : "";
    var rgx = /(\d+)(\d{3})/;
    while (rgx.test(x1)) {
        x1 = x1.replace(rgx, "$1" + "." + "$2");
    }
    return x1 + x2;
}
function calculateTotal($){
    var totalValue = 0;
    $('.input-oh').each(function () {
        if ($(this).val() != ''){
            try {
                var nilai = $(this).val().replace(/\./g,'');
                totalValue = totalValue + eval(nilai);
            } catch (e) {
            }
        }
    });
    return totalValue;
}

jQuery(function ($) {
    $('.input-oh').maskNumber({
        thousands: '.',
        integer: true,
    });
    $.notify("TOTAL OH SEBULAN : \nTOTAL OH HARIAN : ", {
        clickToHide: false,
        autoHide: false,
        style: 'bootstrap',
        className: 'error total-oh',
        globalPosition: 'bottom right',
    });
    $('.input-oh').on('keyup', function () {
        totalValue = calculateTotal($);
        $('#grand-total').html('<strong>TOTAL PER-BULAN : '+ addCommas(totalValue) +'</strong>');
        $('#total-harian').html('<strong>TOTAL PER-HARI : '+ addCommas(Math.round(totalValue/30)) +'</strong>');
        var totalOHView = '<div><div class="pull-left"><strong>TOTAL PER-BULAN : </strong>&nbsp;&nbsp;</div><div class="pull-right"><strong>'+ addCommas(totalValue) +'</strong></div></div><br>';
        totalOHView += '<div><div class="pull-left"><strong>TOTAL PER-HARI : </strong>&nbsp;&nbsp;</div><div class="pull-right"><strong>'+ addCommas(Math.round(totalValue/30)) +'</strong></div></div><br>';
        $('.notifyjs-container .total-oh span').html(totalOHView);
    });
    $('.input-oh').keyup();
})