<script>
$(document).ready(function(){

	$('#escolas').unbind('click');
	$('#escolas').click(function(){
		var checked = $(this).attr('checked');
		$('.escola').each(function(){
			if($(this).attr('checked') == checked){
    			$(this).click();
			}
		});
	});
	$('.escola').unbind('click');
    $('.escola').click(function(){

    	var entid = $(this).attr('entid');
    	var sbaid = <?php echo $dados['sbaid'];?>;
    	var ano   = '2016';
    	var url   = window.location.href;
        var habilitado = '<?php echo $disabled != '' ? 'false' : 'true'; ?>';

    	$.ajax({
            method: "POST",
            url: url,
            data: {'req' : 'vinculaEscolaSubacao', 'sbaid' : sbaid, 'ano' : '2016', 'entid' : entid},
            success: function() {

    		    var url = window.location.href+'&req=listaEscolasIniciativa'+'&habilitado='+habilitado;
    		    $('#div_escolasIniciativa').load(url+'&sbaid='+sbaid);
    		    url = window.location.href+'&req=formItensIniciativaEscola'+'&habilitado='+habilitado;
    		    $('#div_ItensIniciativaEscola').load(url+'&sbaid='+sbaid);
    	    }
    	})
    });
});
</script>
<table class="table" cellspacing="0" width="100%" id="tabelaEscolas">
    <?php
    if(count($arrEscolas) > 0){
    ?>
    <thead>
        <tr>
            <th><input type=checkbox class=i-checks id=escolas ></th>
            <th>Município</th>
            <th>Escola</th>
            <th>Qtd Alunos</th>
            <th>Qtd Salas</th>
        </tr>
    </thead>
    <tbody>
    <?php
        foreach($arrEscolas as $escola){
        ?>
        <tr>
            <td>
                <input type="checkbox"
                        class="i-checks escola"
                        entid="<?php echo $escola['entid']?>"
                        <?php echo $disabled; ?>
                        <?php echo $escola['checked'] ? 'checked="checked"' : '';?>>
            </td>
            <td><?php echo $escola['mundescricao']?></td>
            <td><?php echo $escola['entnome']?></td>
            <td><?php echo $escola['qtde_alunos']?></td>
            <td><?php echo $escola['qtd_salas']?></td>
        </tr>
        <?php
        }
    }else{
        ?>
    <tbody>
        <tr>
            <td colspan=4 >
                <label style="color: red;">
                    Favor escolher um município.
                </label>
            </td>
        </tr>
        <?php
    }
    ?>
    </tbody>
</table>