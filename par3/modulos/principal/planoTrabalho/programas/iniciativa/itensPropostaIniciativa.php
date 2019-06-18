<script>
$(document).ready(function(){

    $('.item').click(function(){

    	console.log($(this))

        var url = window.location.href+'&req=vinculaItemComposicao';
        var picid = $(this).attr('picid');
        var dicid = $(this).attr('dicid');
        var sbaid = <?php echo $dados['sbaid']?>;
        var habilitado = '<?php echo $disabled != '' ? 'false' : 'true'; ?>';

        $.ajax({
            method: "POST",
            url: url+'&sbaid='+sbaid+'&picid='+picid+'&dicid='+dicid+'&icoano=2016',
            success: function() {
            	<?php if($dados['ppscronograma'] == 2){?>
    		    var url = window.location.href+'&req=formItensIniciativaEscola';
    		    $('#div_ItensIniciativaEscola').load(url+'&sbaid='+sbaid+'&habilitado='+habilitado);
    		    <?php }else{?>
    		    var url = window.location.href+'&req=listaItensIniciativa';
    		    $('#div_ItensIniciativa').load(url+'&sbaid='+sbaid+'&habilitado='+habilitado);
    		    <?php }?>
		    }
    	})
    });
});
</script>
<table class="table">
    <?php
    if(count($arrDadosItensPropostaIniciativa) > 0){
    ?>
    <thead>
        <tr>
            <th></th>
            <th>Descrição</th>
            <th>Unidade</th>
            <th>Valor Unitário</th>
        </tr>
    </thead>
    <tbody>
        <?php
        foreach($arrDadosItensPropostaIniciativa as $item){
        ?>
        <tr>
            <td>
                <input type="checkbox"
                        class="i-checks item"
                        picid="<?php echo $item['picid']?>"
                        dicid="<?php echo $item['dicid']?>"
                        <?php echo $disabled;?>
                        <?php echo $item['icoid'] ? 'checked="checked"' : '';?>>
            </td>
            <td><?php echo $item['picdescricao']?></td>
            <td><?php echo $item['umidescricao']?></td>
            <td><?php echo formata_valor($item['dicvalor'])?></td>
        </tr>
    <?php
        }
    }else{
    ?>
        <tr>
            <td colspan=4 >
                <label style="color: red;">
                    Não há Item de Composição vinculado à proposta dessa iniciativa.
                </label>
            </td>
        </tr>
    <?php
    }
    ?>
    </tbody>
</table>