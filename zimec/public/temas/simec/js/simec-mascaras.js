$(function () {

    $('.datemask').mask("99/99/9999");

    $(".moeda").inputmask("currency", {
        radixPoint: ",",
        groupSeparator: ".",
        digits: 2,
        autoGroup: true,
        prefix: ''
    });
});