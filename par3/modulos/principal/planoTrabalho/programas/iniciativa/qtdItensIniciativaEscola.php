<script>
$(document).ready(function(){

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
            <th>Município</th>
            <th>Escola</th>
            <th>Quantidade de Salas</th>
            <th>Quantidade de Alunos</th>
            <th>Quantidade de Itens</th>
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
            <td><?php echo $item['mundescricao']?></td>
            <td><?php echo $item['escnome']?></td>
            <td><?php echo $item['escqtdsalas']?></td>
            <td><?php echo $item['escqtdalunos']?></td>
            <td>
                <input type="text"
                    class="form-control moeda icoquantidade"
                    name="icoquantidade[<?php echo $item['icoid']?>][<?php echo $item['sesid']?>]"
                    icoid="<?php echo $item['icoid']?>"
                    <?php echo $disabled;?>
                    value="<?php echo $item['seiqtd'] > 0 ? $item['seiqtd'] : 0;?>">
            </td>
        </tr>
    <?php
    }
    ?>
    </tbody>
</table>