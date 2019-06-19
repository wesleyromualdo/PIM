<script>
$(document).ready(function(){


    $('.icoquantidade').keyup(function(){

        var icoid           = $(this).attr('icoid');
        var icovalor        = $('#icovalor_'+icoid).val();
        var icoquantidade   = $(this).val();
        var total_itens     = 0;

        if(!icoquantidade){
        	icoquantidade = 0;
        }
        var total_item      = parseFloat(icovalor)*parseFloat(icoquantidade);

        $('#total_item_'+icoid).val(formataNumero(total_item));

        var temp = 0;
        $('.total_item').each(function(){
        	temp = $(this).val().replace('.','');
        	temp = temp.replace('.','');
        	temp = temp.replace('.','');
        	temp = temp.replace('.','');
        	temp = temp.replace(',','.');
        	total_itens += parseFloat(temp);
        });

        $('#total_itens').val(formataNumero(total_itens));
    });

    function formataNumero(str){
        str = str + '';
        x = str.split('.');
        x1 = x[0]; x2 = x.length > 1 ? ',' + x[1] : '';
        var rgx = /(\d+)(\d{3})/;
        while (rgx.test(x1)) {
            x1 = x1.replace(rgx, '$1' + '.' + '$2');
        }
        return (x1 + x2);
    }
});
</script>
<table class="table">
    <thead>
        <tr>
            <th>Descrição</th>
<!--             <th>Unidade</th> -->
            <th>Valor Unitário</th>
            <th style="width: 15%">Quantidade</th>
            <th style="width: 15%">Total</th>
        </tr>
    </thead>
    <tbody>
    <?php
    $tot_itens = 0;
    foreach($arrDadosItensIniciativa as $item){

        $tot_item = $item['icovalor']*$item['icoquantidade'];
        $tot_itens += $tot_item;
    ?>
        <tr>
            <td><?php echo $item['icodescricao']?></td>
            <!--  <td><?php echo $item['icodescricao']?></td>-->
            <td>
                R$ <?php echo formata_valor($item['icovalor']);?>
                <input type="hidden"
                    id="icovalor_<?php echo $item['icoid']?>"
                    value="<?php echo $item['icovalor']?>" />
            </td>
            <td>
                <input type="text"
                    class="form-control moeda icoquantidade"
                    name="icoquantidade[<?php echo $item['icoid']?>]"
                    icoid="<?php echo $item['icoid']?>"
                    <?php echo $disabled;?>
                    value="<?php echo $item['icoquantidade'] > 0 ? $item['icoquantidade'] : 0;?>">
            </td>
            <td>
                <input type="text"
                    class="form-control moeda total_item"
                    disabled="disabled"
                    id="total_item_<?php echo $item['icoid']?>"
                    value="<?php echo formata_valor($tot_item);?>">
            </td>
        </tr>
    <?php
    }
    ?>
        <tr>
            <td></td>
            <td></td>
            <td><h4>Total:</h4></td>
            <td>
                <input type="text"
                        class="form-control moeda"
                        readonly="readonly"
                        id="total_itens"
                        name="total_itens"
                        value="<?php echo formata_valor($tot_itens);?>">
            </td>
        </tr>
    </tbody>
</table>