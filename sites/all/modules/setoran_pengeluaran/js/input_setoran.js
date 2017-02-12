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
    $('.setoran-value').each(function () {
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
    $('.setoran-value').maskNumber({
        thousands: '.',
        integer: true,
    });
    $.notify("TOTAL SETORAN : ", {
        clickToHide: false,
        autoHide: false,
        style: 'bootstrap',
        className: 'error total-setoran',
        globalPosition: 'bottom right',
    });
    $('.setoran-value').on('keyup', function () {
        totalValue = calculateTotal($);
        var splitStrID = $(this).attr('id').split('-');
        var strID = parseInt(splitStrID[splitStrID.length - 1]);
        var komulatif = 0;
        for (var i = 1;i <= $('.setoran-value').length;i++) {
            var nilaiSetoran = $('.setoran-' + i).val().replace(/\./g, '');
            komulatif = komulatif + parseFloat(eval(nilaiSetoran));
            $('#total-'+ i).html(addCommas(komulatif));
        }
        //$('#total-'+ strID).html(addCommas(komulatif));
        $('#grand-total,#grand-total-2').html('<strong>'+ addCommas(totalValue) +'</strong>');
        var totalSetoranView = '<div><div class="pull-left"><strong>TOTAL SETORAN : </strong>&nbsp;&nbsp;</div><div class="pull-right"><strong>'+ addCommas(totalValue) +'</strong></div></div><br>';
        //totalSetoranView += '<div><div class="pull-left"><strong>TOTAL PER-HARI : </strong>&nbsp;&nbsp;</div><div class="pull-right"><strong>'+ addCommas(Math.round(totalValue/30)) +'</strong></div></div><br>';
        $('.notifyjs-container .total-setoran span').html(totalSetoranView);
    });
    $('.setoran-value').keyup();
})