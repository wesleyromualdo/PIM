function setFilterTop(table, table_) {
    var idTable = table.attr('id');

    $('#'+idTable+' thead tr:eq(0) th:eq(0)').removeClass('sorting_asc');

    $('#'+idTable+' thead tr').clone(true).appendTo( '#'+idTable+' thead' );
    $('#'+idTable+' thead tr:eq(1) th').each( function (i) {
        $(this).html( '<input type="text" placeholder=" persquisar " />' );
        $( 'input', this ).on( 'keyup change', function () {
            if ( table_.column(i).search() !== this.value ) {
                table_.column(i).search( this.value ).draw();
            }
        } );
    } );

    $('#'+idTable+' thead tr:eq(1) th:eq(0)').remove();
    $('#'+idTable+' thead tr:eq(0) th:eq(0)').attr('rowspan', '2')
    $('#'+idTable+' thead tr:eq(1) th').removeClass('sorting').unbind();

    $('#'+idTable+' tbody tr').each(function( index ) {
        $(this).find('td:eq(0)').addClass('text-center');
    });
}

$(document).ready(function () {

});