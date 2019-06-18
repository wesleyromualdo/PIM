<?php

$arCampos = array();


//ver($_POST,d);
if(!empty($_POST['codigo'])) {

    $obj = new  Par3_Model_PropostaIniciativa($_POST['codigo']);
    $dados = $obj->recuperarIniciativa($_POST['codigo']);
}


?>

<div class="ibox-content">
	<form method="post" name="formulario" id="formulario" class="form form-horizontal">

        <input type='hidden' value='<?php echo $_POST['indid']?>' id='indid' name='indid'/>
        <input type='hidden' value='<?php echo $_POST['itrid']?>' id='indid' name='itrid'/>
        <input type='hidden' value='<?php echo $_POST['codigo']?>' id='indid' name='codigo'/>
		<input type='hidden' value='incluir' id='acao' name='acao'/>
		<input type='hidden' value='iniciativa' id='tipo' name='tipo'/>
		<?PHP
        $sql = "SELECT
                    	obiid  as codigo,
                    	obidesc  as descricao
                    FROM par3.objetoiniciativa
                    where obistatus = 'A'
                    ORDER BY 2";

    		echo $simec->select('obiid', 'Tipo de Objeto', $dados['obiid'], $sql, array('placeHolder' => 'Tipo de Objeto', 'maxlengh' => 1200));

            $sql1 = "SELECT
                    	aciid   as codigo,
                    	acidesc   as descricao
                    FROM par3.areascorrelatasiniciativa
                     where acistatus = 'A'
                    ORDER BY 2";

    		echo $simec->select('aciid', 'Areas Relacionadas', $dados['aciid'], $sql1, array('placeHolder' => 'Areas Relacionadas', 'maxlengh' => 1200));

    		$tiposA = array(
    		    "G" => "Global",
    		    "E" => "Por escola",
    		);

    		echo $simec->select('pricronograma', 'Cronograma', $dados['pricronograma'], $tiposA, array('placeHolder' => 'Cronograma', 'maxlengh' => 1200));

    		echo $simec->input('pridesc', 'Descrição', $dados['pridesc'], array('placeHolder' => 'Descrição', 'maxlengh' => 2000));

    		$tiposA = array(
    		    "2016" => "2016",
    		    "2017" => "2017",
    		    "2018" => "2018",
    		    "2019" => "2019",
    		);

    		echo $simec->select('anos[]', 'Anos', array(), $tiposA, array('placeHolder' => 'Anos', 'maxlengh' => 1200, $disabled));

    		echo $simec->textarea('priorientacao', 'Orientação', $dados['priorientacao'], array('placeHolder' => 'Orientação', 'maxlengh' => 1200, $boIndicador));

    		$sql = "SELECT
                    	estuf as codigo,
                    	estdescricao as descricao
                    FROM territorios.estado
                    ORDER BY 2";

    		echo $simec->select('estados[]', 'Estados', array(), $sql, array('placeHolder' => 'Estados', 'maxlengh' => 1200, $disabled));

    		$sql = "SELECT
                    	gtmid as codigo,
                    	gtmdsc as descricao
                    FROM territorios.grupotipomunicipio
                    WHERE
                    	gtmstatus = 'A'
                    ORDER BY 2";

    		echo $simec->select('grupo_municipio[]', 'Grupo de Municipios', array(), $sql, array('placeHolder' => 'Grupo de Municipios', 'maxlengh' => 1200, $disabled));

		?>

		<div class="form-group">
			<div class="col-lg-12 col-sm-offset-3">
			<?php if($_POST['acaoGuia'] == 'incluir'){ ?>
				<button type="submit" class="btn btn-success">
					<span class="fa fa-save"> </span> Salvar
				</button>
			<?php } else { ?>
				<button type="submit" class="btn btn-success">
					<span class="fa fa-edit"> </span> Alterar
				</button>
			<?php } ?>
			</div>
		</div>
	</form>
	</div>

<?php die(); ?>