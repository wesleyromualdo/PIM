<?php

include_once 'programas/_funcoes_maismedicos.php';
/**
 * Tela de dados do prefeito
 *
 * @category visao
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01/10/2015
 * @link     no link
 */
global $simec;

if ($_REQUEST['requisicao'] == 'carregaMunicipios') {
    ob_clean();
    $municipio = new Territorios_Model_Municipio();
    print simec_json_encode($municipio->lista(array('muncod', 'mundescricao'), array("estuf = '{$_REQUEST['estuf']}'")));
    die;
}

$modelProadesao = new Par3_Model_Proadesao();
$proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($_REQUEST['prgid']);
$dtFinal = (int) str_replace('-','',$proadesaoDados['pfadatafinal']);
$dtAtual =  (int) str_replace('-','',date('Y-m-d'));
$somenteLeitura = ($dtFinal > 0 && ($dtAtual > $dtFinal))? true : false;



$estado = new Territorios_Model_Estado();

#define sexo masculino e deminino no radio button
$radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');


$municipio = $controleUnidade->pegarMunicipio($_REQUEST['inuid']);

$renderEntidade = new Par3_Controller_Entidade();
$controllerInstrumentoUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$muncod = $controleUnidade->pegarMuncod($_REQUEST['inuid']);


$prgid = $_REQUEST['prgid'];
$inuid = $_REQUEST['inuid'];
$tenid = $_REQUEST['tenid'] = Par3_Model_InstrumentoUnidadeEntidade::PREFEITO;

$dados_getor_s = buscaDadosGestorMaisMedicos('S', $muncod, $prgid);
$dados_getor_m = buscaDadosGestorMaisMedicos('M', $muncod, $prgid);

$radioSexo = array('f' => 'Feminino', 'm' => 'Masculino');

if ($_REQUEST['req'] == 'salvarDadosMaisMedicos') {
    salvarDadosMaisMedicos($_REQUEST);
}


switch ($_REQUEST['req']) {
    case 'salvar':
        $controllerInstrumentoUnidadeEntidade->salvarEntidade($_POST);
        die();
        break;
    default:
        $objPessoaFisica = $modelInstrumentoUnidadeEntidade->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco = new Par3_Model_Endereco($objPessoaFisica->endid);
        $arrParamHistorico = array();
        $arrParamHistorico['inuid'] = $inuid;
        $arrParamHistorico['tenid'] = $tenid;
        $arrParamHistorico['booCPF'] = true;
        $disabled = 'disabled="disabled"';
        $display = Array(
            'entdtnascimento' => 'N',
            'enttelefonecelular' => 'N',
            'enttelefonefax' => 'N',
        );
        break;
}

#SALVA DADOS MAIS MEDICOS. GETOR SUS E GETOR LOCAL "MUNICIPAL"
function salvarDadosMaisMedicos($dados)
{
    global $db;

    extract($dados);
    $inuid = $_REQUEST['inuid'];
    $dapdtnascimento_s = formata_data_sql($dapdtnascimento_s);
    $dapcpf_s = corrige_cpf($dapcpf_s);
    $dapdtnascimento_m = formata_data_sql($dapdtnascimento_m);
    $dapcpf_m = corrige_cpf($dapcpf_m);

    if ($dapid_s == '' || $dapid_m == '') {
        $sql = "
                INSERT INTO maismedicomec.dadosadesaorepresentante(
                        prgid, muncod, muncodend, dapnome, dapcpf, daprg, dapsexo, dapdtnascimento, daporgao, dapfonecomercial,
                        dapcelular, dapemail, dapcargofuncao, daptipo, dapstatus, dapdtinclusao
                    )VALUES(
                        '{$prgid}', '{$muncod}', '{$muncod_s}', '{$dapnome_s}', '{$dapcpf_s}', '{$daprg_s}', '{$dapsexo_s}', '{$dapdtnascimento_s}', 
                        '{$daporgao_s}', '{$dapfonecomercial_s}', '{$dapcelular_s}', '{$dapemail_s}', '{$dapcargofuncao_s}', 'S', 'A', NOW()
                );
            ";

        $sql .= "
                INSERT INTO maismedicomec.dadosadesaorepresentante(
                        prgid, muncod, muncodend, dapnome, dapcpf, daprg, dapsexo, dapdtnascimento, daporgao, dapfonecomercial, dapcelular, 
                        dapemail, dapcargofuncao, daptipo, dapstatus, dapdtinclusao
                    )VALUES(
                        '{$prgid}', '{$muncod}', '{$muncod_m}', '{$dapnome_m}', '{$dapcpf_m}', '{$daprg_m}', '{$dapsexo_m}', '{$dapdtnascimento_m}', '{$daporgao_m}', 
                        '{$dapfonecomercial_m}', '{$dapcelular_m}', '{$dapemail_m}', '{$dapcargofuncao_m}', 'M', 'A', NOW()
                )RETURNING dapid;
            ";
        $dapid = $db->pegaUm($sql);

    } else {
        $sql = "
                UPDATE maismedicomec.dadosadesaorepresentante SET
                    muncodend	= '{$muncod_s}', 
                    dapnome		= '{$dapnome_s}', 
                    dapcpf		= '{$dapcpf_s}', 
                    daprg		= '{$daprg_s}', 
                    dapsexo		= '{$dapsexo_s}', 
                    dapdtnascimento = '{$dapdtnascimento_s}', 
                    daporgao	= '{$daporgao_s}', 
                    dapfonecomercial= '{$dapfonecomercial_s}', 
                    dapcelular	= '{$dapcelular_s}', 
                    dapemail	= '{$dapemail_s}', 
                    dapcargofuncao	= '{$dapcargofuncao_s}'
                WHERE dapid = {$dapid_s} AND muncod = '{$muncod}' AND prgid = '{$prgid}' RETURNING dapid;
            ";

        $sql .= "
                UPDATE maismedicomec.dadosadesaorepresentante SET
                    muncodend	= '{$muncod_m}', 
                    dapnome		= '{$dapnome_m}', 
                    dapcpf		= '{$dapcpf_m}', 
                    daprg		= '{$daprg_m}', 
                    dapsexo		= '{$dapsexo_m}', 
                    dapdtnascimento = '{$dapdtnascimento_m}', 
                    daporgao	= '{$daporgao_m}', 
                    dapfonecomercial= '{$dapfonecomercial_m}', 
                    dapcelular	= '{$dapcelular_m}', 
                    dapemail	= '{$dapemail_m}', 
                    dapcargofuncao	= '{$dapcargofuncao_m}'
                WHERE dapid = {$dapid_m} AND muncod = '{$muncod}' AND prgid = '{$prgid}' RETURNING dapid;
            ";
        $dapid = $db->pegaUm($sql);
    }

    if ($dapid > 0) {
        $db->commit();
        header("Location: par3.php?modulo=principal/adesao/termo&acao=A&prgid={$_REQUEST['prgid']}&inuid={$_REQUEST['inuid']}&aba=dados");
        $_SESSION['maismedicomec']['msg'] = "Dados salvos com sucesso.";
        die();
    }
}

?>

<form method="post" name="formularioDadosRepresentante" id="formularioDadosRepresentante" class="form form-horizontal">
    <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>
    <input type="hidden" name="aba" id="aba" value="dados"/>
    <input type="hidden" name="req" value="salvarDadosMaisMedicos"/>
    <input type="hidden" name="muncod" id="muncod" value="<?php echo $muncod; ?>"/>
    <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
    <input type="hidden" name="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
    <input type="hidden" id="dapid_s" name="dapid_s" value="<?php echo $dados_getor_s['dapid']; ?>"/>
    <input type="hidden" id="dapid_m" name="dapid_m" value="<?php echo $dados_getor_m['dapid']; ?>"/>
    <div class="ibox">
        <div class="ibox-title">
            <div class="row">
                <div class="col-md-12">
                    <h3 id="entidade">DADOS DO(A) PREFEITO(A) DE - <?php echo strtoupper($municipio); ?></h3>
                </div>
            </div>
        </div>
        <div class="ibox-content" id="div_prefeito">
            <?php $renderEntidade->formPessoaFisica($disabled, $objPessoaFisica, null, $display); ?>
        </div>

        <div class="ibox-title">
            <h3> Dados do Gestor Local do Sistema Único de Saúde (SUS)</h3>
        </div>

        <div class="ibox-content">
            <?php echo $simec->cpf('dapcpf_s', 'CPF', $dados_getor_s['dapcpf'], array('class' => 'cpf', 'required', 'data-pessoa' => true, 'data-pessoa-campos' => '{"#dapnome_s": "no_pessoa_rf"}')); ?>
            <?php echo $simec->input('dapnome_s', 'Nome Completo', $dados_getor_s['dapnome'], array('required')); ?>
            <?php echo $simec->input('daprg_s', 'RG', $dados_getor_s['daprg'], array('required')); ?>
            <?php echo $simec->radio('dapsexo_s', 'Sexo', $dados_getor_s['dapsexo'] ? strtolower($dados_getor_s['dapsexo']) : '', $radioSexo, array('checked', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline', 'required')); ?>
            <?php echo $simec->data('dapdtnascimento_s', 'Data de Nascimento', $dados_getor_s['dapdtnascimento'], array('required')); ?>
            <?php echo $simec->select('estuf_s', 'UF', $dados_getor_s['estuf'], $estado->pegarSQLSelect($_REQUEST), array('required')); ?>
            <?php echo $simec->select('muncod_s', 'Municipio', $dados_getor_s['muncodend'], "SELECT   muncod as codigo, mundescricao as descricao FROM territorios.municipio where estuf = '{$dados_getor_s['estuf']}'", array('data-placeholder' => 'Município', 'required')); ?>
            <?php echo $simec->input('daporgao_s', 'Órgão', $dados_getor_s['daporgao'], array('required')); ?>
            <?php echo $simec->telefone('dapfonecomercial_s', 'Telefone Fixo Comercial', $dados_getor_s['dapfonecomercial'], array('class' => 'telefone', 'required')); ?>
            <?php echo $simec->telefone('dapcelular_s', 'Telefone Celular', $dados_getor_s['dapcelular'], array('class' => 'celular', 'required')); ?>
            <?php echo $simec->email('dapemail_s', 'E-Mail', $dados_getor_s['dapemail'], array('class' => 'email', 'type' => 'email', 'required')); ?>
            <?php echo $simec->input('dapcargofuncao_s', 'Cargo/Função:', $dados_getor_s['dapcargofuncao'], array('required')); ?>
        </div>

        <div class="ibox-title">
            <h3> Dados do Representante Legal do Município no Acompanhamento desde Edital</h3>
        </div>

        <div class="ibox-content">
            <?php echo $simec->cpf('dapcpf_m', 'CPF', $dados_getor_m['dapcpf'], array('class' => 'cpf', 'required', 'data-pessoa' => true, 'data-pessoa-campos' => '{"#dapnome_m": "no_pessoa_rf"}')); ?>
            <?php echo $simec->input('dapnome_m', 'Nome Completo', $dados_getor_m['dapnome'], array('required')); ?>
            <?php echo $simec->input('daprg_m', 'RG', $dados_getor_m['daprg'], array('required')); ?>
            <?php echo $simec->radio('dapsexo_m', 'Sexo', $dados_getor_m['dapsexo'] ? strtolower($dados_getor_m['dapsexo']) : '', $radioSexo, array('checked', 'required'), array('type' => 'radio radio-info radio', 'style' => 'inline')); ?>
            <?php echo $simec->data('dapdtnascimento_m', 'Data de Nascimento', $dados_getor_m['dapdtnascimento'], array('required')); ?>
            <?php echo $simec->select('estuf_m', 'UF', $dados_getor_m['estuf'], $estado->pegarSQLSelect($_REQUEST), array('required')); ?>
            <?php echo $simec->select('muncod_m', 'Municipio', $dados_getor_m['muncodend'], "SELECT   muncod as codigo, mundescricao as descricao FROM territorios.municipio where estuf = '{$dados_getor_m['estuf']}'", array('data-placeholder' => 'Município', 'required')); ?>
            <?php echo $simec->input('daporgao_m', 'Órgão', $dados_getor_m['daporgao'], array('required')); ?>
            <?php echo $simec->telefone('dapfonecomercial_m', 'Telefone Fixo Comercial', $dados_getor_m['dapfonecomercial'], array('class' => 'telefone', 'required')); ?>
            <?php echo $simec->telefone('dapcelular_m', 'Telefone Celular', $dados_getor_m['dapcelular'], array('class' => 'celular', 'required')); ?>
            <?php echo $simec->email('dapemail_m', 'E-Mail', $dados_getor_m['dapemail'], array('class' => 'email', 'type' => 'email', 'required')); ?>
            <?php echo $simec->input('dapcargofuncao_m', 'Cargo/Função:', $dados_getor_m['dapcargofuncao'], array('required')); ?>
        </div>

        <div class="ibox-footer center">

            <?php
            if (!Par3_Model_UsuarioResponsabilidade::perfilConsulta() && !$somenteLeitura) {
                $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabledForm);
                ?>
                <button type="submit" class="btn btn-success salvar">
                    <i class="fa fa-save"></i> Salvar Dados
                </button>
            <?php } ?>

        </div>
    </div>
</form>

<br>
<br>
<script>
    jQuery(document).ready(function () {
        $('.table').removeClass('table-hover');

        <?php if ($_SESSION['maismedicomec']['msg'] != ''){ ?>
        alert('<?php echo $_SESSION['maismedicomec']['msg']; ?>');
        <?php unset($_SESSION['maismedicomec']['msg']); } ?>

        var muncod_hid = jQuery('[name="muncod_hid"]').val();

        jQuery('#muncod_s').append(new Option('', ''));
        jQuery("#dapsexo_s").prop('checked');
        jQuery("#dapsexo_s").attr('checked', 'checked');

        if (jQuery('[name="estuf_s"]').val() != '') {
            carregarMunicipio(jQuery('[name="estuf_s"]').val(), muncod_hid);
        }

        jQuery('[name="estuf_s"]').change(function () {
            if (jQuery('[name="esfera"]').val() != 'E') {
                var estuf_s = jQuery(this).val();
                carregarMunicipio(estuf_s, muncod_hid, 'muncod_s');
            }
        });

        jQuery('[name="estuf_m"]').change(function () {
            if (jQuery('[name="esfera"]').val() != 'E') {
                var estuf_m = jQuery(this).val();
                carregarMunicipio(estuf_m, muncod_hid, 'muncod_m');
            }
        });

        jQuery('.export').click(function () {
            window.open(window.location.href + "&requisicao=exportar&" + jQuery('#formulario').serialize());
        });
        <?php if($somenteLeitura){ ?>
            jQuery("input,select,textarea").not('#chosen-sistema').attr('disabled', 'disabled');
            setTimeout(function(){
                jQuery('.search-choice-close').hide();
                jQuery('.chosen-disabled').children().css('background-color','#EEEEEE');
            },200);
        <?php } ?>
    });

    function carregarMunicipio(estuf, muncod_hid, campo) {
        if (estuf != '') {
            var options = jQuery('#' + campo);
            jQuery.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=carregaMunicipios&estuf=" + estuf,
                async: false,
                success: function (result) {
                    options.empty();
                    options.append(new Option('', ''));

                    var result = JSON.parse(result);
                    $.each(result, function () {
                        options.append(new Option(this.mundescricao, this.muncod));
                    });
                    options.focus();
                    if (muncod_hid) {
                        options.val(muncod_hid);
                    }
                    options.trigger('chosen:updated');
                }
            });
        }
    }
</script>

