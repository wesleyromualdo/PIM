<?php
/**
 * Lista de Municipios
 *
 * @category Lista
 * @package  A1
 * @author   Marcus Rocha <marcusrocha@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  SVN:
 * @link     no link
 */
require APPRAIZ . 'includes/cabecalho.inc';
require APPRAIZ . 'includes/Agrupador.php';

$renderNutricionista = new Par3_Controller_Entidade();
$controleUnidade     = new Par3_Controller_InstrumentoUnidade();

switch ($_REQUEST['req']) {
    case 'salvarDadosPessoais':

        $endereco = new Par3_Model_Endereco($_POST['endid']);
        $endereco->preparaDadosGravacao();
        $endereco->popularDadosObjeto($_POST);
        $_POST['endid'] = $endereco->salvar();
        $endereco->commit();

        $dadosPessoais = new Par3_Model_DadosNutricionista($_POST['usucpf']);
        $dadosPessoais->preparaDadosGravacao();
        $_POST['dandatanascimento'] = formata_data_sql($_POST['dandatanascimento']);
        $dadosPessoais->popularDadosObjeto($_POST);
        $dadosPessoais->salvar();
        $dadosPessoais->commit();

        $vinculacao = new Par3_Model_VinculacaoNutricionista();
        $vinculacao->finalizarCadastro();

        $url = "par3.php?modulo=principal/nutricionista/cadNutricionista&acao=A";
        simec_redirecionar($url, 'success');

        break;
    case 'buscaMunicipiosChosen':

        $municipio = new Territorios_Model_Municipio();

        $arrMunicipios = $municipio->recuperarTodos('*',Array("estuf = '{$_REQUEST['estuf']}'"));
        $arrMunicipios = is_array($arrMunicipios) ? $arrMunicipios : Array();

        ob_clean();
        echo '<option value label selected=selected ></option>';
        foreach($arrMunicipios as $municipio){
            echo "<option value={$municipio['muncod']} label='{$municipio['mundescricao']}'>{$municipio['mundescricao']}</option>";
        }
        die();

        break;
    default:
        $vinculacao     = new Par3_Model_VinculacaoNutricionista();
        $dadosVinculcao = $vinculacao->recuperaDadosVinculacao();

        $usuario                 = new Seguranca_Model_Usuario($_SESSION['usucpf']);
        $modelDadosnutricionista = new Par3_Model_DadosNutricionista();
        $modelDadosnutricionista->populateData($usuario);
        //$modelDadosnutricionista->carregarDadosPorCPF($_SESSION['usucpf']);
        $objEndereço             = new Par3_Model_Endereco($modelDadosnutricionista->endid);

        $abas = array(
            0 => array("descricao" => "Vinculação",     "link" => "par3.php?modulo=principal/nutricionista/vinculacaoNutricionista&acao=A"),
            1 => array("descricao" => "Dados Pessoais", "link" => "par3.php?modulo=principal/nutricionista/cadNutricionista&acao=A"),
            2 => array("descricao" => "Desvinculação",  "link" => "par3.php?modulo=principal/nutricionista/desvinculacaoNutricionista&acao=A"),
        );
        break;
}

echo $simec->title('Lista Cadastro Nutricionista', '');
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <form method="post" name="formulario" id="formulario" class="form form-horizontal">
            <input type="hidden" name="req" value="salvarDadosPessoais"/>
            <input type="hidden" name="danid" id="danid" value="<?php echo $modelDadosnutricionista->danid ?>"/>
            <?php echo $simec->tab($abas, "par3.php?modulo=principal/nutricionista/cadNutricionista&acao=A"); ?>
            <div class="ibox">
                <div class="ibox-title">
                    <h3>Dados do(a) Nutricionista</h3>
                </div>
                <div class="ibox-content">
                    <?php
                    if(!empty($dadosVinculcao)) {
                        $dataVal = formata_data($modelDadosnutricionista->dandatanascimento);
                        $disabled        = false;
                        $radioSexo       = array('f' => 'Feminino', 'm' => 'Masculino');
                        $radioProvisorio = array('true' => 'Sim', 'false' => 'Não');

                        echo $simec->cpf('dancpf', 'CPF', $usuario->usucpf, array('class' => 'cpf', 'readonly', 'data-pessoa' => true, 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}'));
                        echo $simec->input('dannome', 'Nome', $usuario->usunome, array('maxlength' => '255', 'disabled', 'required', 'readonly' => 'readonly'));

                        echo $simec->data('dandatanascimento', 'Data Nascimento', $dataVal, array($disabled, 'required'));
                        echo $simec->input('dannomemae', 'Nome da mãe', $modelDadosnutricionista->dannomemae, array('required' => 'required', 'maxlength' => '100'));
                        echo $simec->radio('dansexo', 'Sexo', $modelDadosnutricionista->dansexo ? $modelDadosnutricionista->dansexo : 'm', $radioSexo, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                        echo $simec->input('dancrn', 'CRN', trim($modelDadosnutricionista->dancrn), array('required' => 'required', 'max' => '9999999999999', 'type' => 'number'));
                        echo $simec->input('dancrnuf', 'UF da CRN', $modelDadosnutricionista->dancrnuf, array('required' => 'required', 'maxlength' => '2'));
                        echo $simec->radio('dancrnprovisorio', 'CRN provisório?', $modelDadosnutricionista->dancrnprovisorio == 't' ? 'true' : 'false', $radioProvisorio, array($disabled, 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline'));
                        echo $simec->email('danemailprincipal', 'E-mail', $modelDadosnutricionista->danemailprincipal, array('class' => 'email', $disabled, 'required'));
                        echo $simec->email('danemailalternativo', 'E-mail alternativo', $modelDadosnutricionista->danemailalternativo, array('required' => 'required', 'maxlength' => '100'));
                        echo $simec->telefone('dantelefonefixo', 'Telefone Comercial', $modelDadosnutricionista->dantelefonefixo, array('class' => 'telefone', 'required', $disabled));
                        echo $simec->telefone('dantelefonecelular', 'Telefone Celular', $modelDadosnutricionista->dantelefonecelular, array('class' => 'telefone', 'required', $disabled));

                        $renderNutricionista->formEnderecoEntidade($disabled, $objEndereço);

                        $sql = "SELECT
                                    estuf as codigo,
                                    estdescricao as descricao
                                FROM territorios.estado";

                        echo $simec->select("estuf", 'UF', $modelDadosnutricionista->estuf, $sql, array('maxlength' => '255', null));

                        if($modelDadosnutricionista->estuf != ''){
                            $sql = "SELECT
                                        muncod as codigo,
                                        mundescricao as descricao
                                    FROM territorios.municipio
                                    WHERE
                                        estuf = '{$modelDadosnutricionista->estuf}'";

                            echo $simec->select("muncod", 'Município', $modelDadosnutricionista->muncod, $sql, array('maxlength' => '255', null));
                        }else{
                            $sql = "SELECT
                                        muncod as codigo,
                                        mundescricao as descricao
                                    FROM territorios.municipio
                                    LIMIT 1";

                            echo $simec->select("muncod", 'Município', '', $sql, array('maxlength' => '255', 'disabled'));
                        }
                    }else{?>
                             Para prosseguir com o cadastro de nutricionista é necessário validar todos os vínculos que você tem com os estados/municípios.
                        <?
                    }
                    ?>
                </div>
                <div class="ibox-footer">
                    <div class="row">
                        <div class="col-lg-3"></div>
                        <div class="col-lg-9">
                        <?php if(!empty($dadosVinculcao)) {?>
                        <div>
                            <button type="submit" class="btn btn-success salvar"><i class="fa fa-save"></i> Salvar</button>
                        </div>
                        <?php } ?>
                        </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
$(document).ready(function(){

    $('#endcep').blur(function(){

        var cep        = $(this).val();
        var logradouro = '';

        url = '/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP';
    	data = '&endcep=' + cep;
    	$.post(
    		url
    		, data
    		, function(resp){

    			var dados = resp.split("||");
    			logradouro = dados[0];

                if(cep != '' && logradouro == ''){
                    $('#estuf').parent().parent().find('.control-label').html('UF: *');
                    $('#muncod').parent().parent().find('.control-label').html('Município: *');
                    swal('Endereço não encontrado.', 'Favor preencher os campos manualmente, incluindo UF e Município.', 'error');
        			$('#estuf').attr('required', 'required');
        			$('#muncod').attr('required', 'required');
                }else{
                    $('#estuf').parent().parent().find('.control-label').html('UF: ');
                    $('#muncod').parent().parent().find('.control-label').html('Município: ');
        			$('#estuf').attr('required', false);
        			$('#muncod').attr('required', false);
                }
    		}
    	);


        $('#estuf, #muncod').trigger("chosen:updated");
    });

	$('#estuf').change(function(){

		var estuf = $(this).val();

		if(estuf != ''){
			$('#muncod').attr('disabled', false);
    		$.ajax({
                type: 'post',
                url: window.location.href,
                data: 'req=buscaMunicipiosChosen&estuf='+estuf,
                async: false,
                success: function (res) {
                	$('#muncod').html(res);
                }
            });
		}else{
			$('#muncod').val('');
			$('#muncod').attr('disabled', true);
		}

        $('#muncod').trigger("chosen:updated");
	});

    <?php //Fix to populate date ?>
    <?php if (!empty($dataVal)) : ?>
        $("#dandatanascimento").val("<?= $dataVal; ?>");
    <?php endif; ?>
});
</script>