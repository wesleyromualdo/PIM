<?php
/**
 * View da representando a tabela aviso
 *
 * @category visao
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 01-03-2016
 * @link     no link
 */

include_once APPRAIZ . 'includes/cabecalho_bootstrap_v3.inc';

$sisid = $_SESSION['sisid'];

$controllerAviso     = new Sac_Controller_Aviso();
$modelAviso          = new Sac_Model_Aviso($_GET['aviid']);
$modelAvisoArquivo   = new Sac_Model_AvisoArquivo();

switch ($_REQUEST['action']) {
	case 'salvar':
	    $controllerAviso->salvar($_REQUEST,$_FILES);
	    break;
	case 'inativar':
	    $id = $_GET['aviid'];
	    $controllerAviso->inativar($id);
	    break;
        case 'downloadArquivo': 
            $controllerAviso->downloadArquivo($_REQUEST['arqid']);
            break;
        case 'excluirArquivo':
            $modelAvisoArquivo->excluirPorArqid($_REQUEST['arqid']);
            simec_redirecionar('par3.php?modulo=sistema/aviso/formulario&acao=A&aviid='.$_REQUEST['aviid'], 'success', 'Arquivo excluído.');
            break;
}
$rowaarq      = $modelAvisoArquivo->pegarArqid($modelAviso->aviid);
$localizacoes = $controllerAviso->recuperarFiltros($modelAviso->aviid);
$perfis = empty(current($localizacoes['perfil'])) ? 0 : 1;
$regioes = empty(current($localizacoes['regiao'])) ? 0 : 1;
$estados = empty(current($localizacoes['estado'])) ? '' : "['".implode("', '", $localizacoes['estado'])."']";
$municipios = empty(current($localizacoes['municipio'])) ? '' : "['".implode("', '", $localizacoes['municipio'])."']";

$podeEditar = $modelAviso->aviid ? 'N' : 'S';
?>
<form method="post" name="formulario-aviso" id="formulario-aviso" class="form form-horizontal" enctype="multipart/form-data">
    <input type="hidden" name="action" value="salvar" />
    <input type="hidden" name="aviid" id="aviid" value="<?php echo $modelAviso->aviid ?>" />
    <input type="hidden" name="sisid" id="sisid" value="<?php echo $sisid ?>" />

    <div class="row">
        <div class="col-md-5">
            <div class="ibox">
                <div class="ibox-title">
                    <h3>Para Quem</h3>
                </div>
                <div class="ibox-content">
                    <?php
                    $sql = "select pflcod as codigo, pfldsc as descricao from seguranca.perfil where sisid = $sisid order by descricao";
                    echo $simec->select('pflcod[]', 'Perfil', $localizacoes['perfil'], $sql, null, array('pode-editar'=>$podeEditar));
                    echo $simec->select('estuf[]', 'UF', $localizacoes['estado'], array('territorios.estado', 'estuf', "estuf || ' - ' || estdescricao"), array('id'=>'id_uf'), array('pode-editar'=>$podeEditar));
                    $sql = "select muncod as codigo, estuf || ' - ' ||mundescricao as descricao from territorios.municipio where estuf in ('" . implode("', '", $localizacoes['estado']) . "')";
                    echo $simec->select('muncod[]', 'Município', $localizacoes['municipio'], $sql, array('id'=>'id_municipio'), array('pode-editar'=>$podeEditar));
                    ?>
                </div>
            </div>
        </div>
        <div class="col-md-7">
            <div class="ibox">
                <div class="ibox-title">
                    <h3>Comunicações</h3>
                </div>
                <div class="ibox-content">
                    <?php
                    echo $simec->periodo('avidata', 'Data de Exibição', array($modelAviso->avidatainicio, $modelAviso->avidatafim), null);
                    echo $simec->radio('avitipoexibicao', 'Tipo de Exibição', $modelAviso->avitipoexibicao ? $modelAviso->avitipoexibicao : 'M', array('M'=>'Modal', 'T'=>'Tarja na tela', 'A'=>'Somente Aviso'), array('required'));
                    if (!empty($rowaarq)) {
                        $linkarq = "";
                        foreach ($rowaarq as $row){
                            $linkarq .= "<a style='padding: 0px;' class='btn' href='par3.php?modulo=sistema/aviso/formulario&acao=A&aviid={$_REQUEST['aviid']}&action=downloadArquivo&arqid={$row['arqid']}' target='blank'><small style='font-size: 12px;'><i class='fa fa-download'></i> {$row['avadsc']}</small></a>";
                            $linkarq .= "<a href=\"#\" onclick=\"excluirArquivo( ".$row['arqid']." );\" title=\"Excluir Anexo\">
                                        <img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\">
                                      </a> <br/>";
                        }
                        echo $simec->input('arqid_old[]', 'Anexo', null, array('type'=>'file','multiple'=>'','help' => "{$linkarq}"));
                    } else {
                        echo $simec->input('arqid_old[]', 'Anexo', null, array('type'=>'file','multiple'=>''));
                    }
                    echo $simec->input('avititulo', 'Título', $modelAviso->avititulo, array('required'=>'required', 'maxlength'=>'200') );
                    echo $simec->textArea('avidsc', 'Texto', $modelAviso->avidsc, array('class'=>'summernote'));
                    ?>
                </div>
                <div class="text-center ibox-footer">
                    <button type="submit" id="btn-enviar" class="btn btn-primary" >Salvar</button>
                    <a href="par3.php?modulo=sistema/aviso/index&acao=A" class="btn btn-danger">Voltar</a>
                </div>
            </div>
        </div>
    </div>
</form>

<div style="clear: both; padding-bottom: 50px;"></div>

<script type="text/javascript">
    $(function(){
        $('#btn-enviar').click(function(){
            if(!$('#aviid').val() && (!$('#pflcod :selected').length && !$('#id_uf :selected').length && !$('#id_municipio :selected').length)){
                swal({
                    title: "Atenção",
                    text: "Você Não selecionou nenhum filtro de usuários.\n\r Isso enviará um comunicado para todos os usuários do módulo.\n\r\n\rDeseja continuar?",
                    type: "warning",
                    showCancelButton: true,
                    confirmButtonColor: "#18a689",
                    cancelButtonColor: "#ec4758",
                    cancelButtonText: "Não",
                    confirmButtonText: "Sim",
                    closeOnConfirm: false
                }, function(){
                    $('#formulario-aviso').submit();
                });
                return false;
            }
        });

        $('#formulario-aviso').on('change', '#id_uf', function(){
            carregarSelectPorJson(
                '/publico/requisicoes.php?&action=carregarMunicipio&estuf='+ $('#id_uf').val(),
                '#id_municipio', 'muncod', 'mundescricao', null, 'Selecione');
        });
    });
    function excluirArquivo(arqid){
	var url = '?modulo=sistema/aviso/formulario&acao=A';
        $(window).attr('location', url+'&aviid=<?php echo $_REQUEST['aviid']; ?>&action=excluirArquivo&arqid='+arqid);
    }
</script>