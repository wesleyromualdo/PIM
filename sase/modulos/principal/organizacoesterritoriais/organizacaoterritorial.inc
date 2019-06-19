<?php


include_once '../../sase/classes/Informacoesterritorio.class.inc';
include_once '../../sase/classes/Contatoorganizacao.class.inc';
include_once '../../sase/classes/Territoriomunicipio.class.inc';
include_once '../../sase/classes/TipoOrgao.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';

global $db;
$Territorio = new Territorio();
$Informacoesterritorio = new Sase_Model_Informacoesterritorio();
$contatoOrganizacao = new ContatoOrganizacao();
$Territoriomunicipio = new Territoriomunicipio();
$tipoOrgao = new TipoOrgao();
$estPre = false;

if($_GET['requisicao']  == 'downloadAnexo'){
    $file = new FilesSimec("documentoterritorio", NULL, "sase");
    $file->getDownloadArquivo($_GET['arqid']);
}


if (!empty($_GET['infterid'])) {
    $orgTerId = $_GET['infterid'];
    $sql = "SELECT * from sase.informacoesterritorio i 
            LEFT JOIN sase.contatoorganizacao c on (c.infterid=i.infterid)
            LEFT JOIN sase.tipoorgao t on (t.infterid = i.infterid)
            LEFT JOIN entidade.entidade e on (e.entid = t.entid)
            WHERE i.infterid = {$_GET['infterid']}";
    $arrDados = $db->pegaLinha($sql);
    //ver($_GET,$sql,$arrDados,d);


} else {
    $orgTerId = '';
}

if (!empty($arrDados['inftercor'])) {
    $cor = $arrDados['inftercor'];
} else {
    $cor = "#000000";
}

if ($_GET['filtroAjaxcorOrganizacao']) {
    ob_clean();
      if (isset($_GET['cor']) && ($_GET['orgid'] > 0) ) {
       
        $sql = "SELECT count(*)
        FROM sase.informacoesterritorio
        WHERE orgid = '{$_GET['orgid']}' AND inftercor = '{$_GET['cor']}'";
        $retorna = $db->pegaUm($sql);
        echo $retorna;
    } 
    die();
}

if ($_GET['filtroAjaxcor']) {
    ob_clean();
    if ($_GET['cor']) {
        $sql = "SELECT COUNT (inftercor) FROM sase.informacoesterritorio
        where inftercor =  '{$_GET['cor']}'  AND infterid <> {$_GET['infterid']}";
        $retornacor = $db->pegaUm($sql);
        echo json_encode($retornacor);
    }
    die();
}

if ($_GET['filtroAjaxTerritorioMunicipio']) {
    ob_clean();
    if ($_GET['infterid']) {
        $sql = "SELECT muncod FROM sase.territoriomunicipio
        where muncod =  '{$_POST['muncod']}'";
        $retornaMunicipio = $db->pegaUm($sql);

        echo json_encode($retornaMunicipio);
    }
    die();
}

if ($_POST['filtroAjaxDocumento']) {
    ob_clean();

    if ($_FILES['arquivo']['size'] > 0) {
        $campos = array(
            "docterinstrumentojuridico" =>  $_POST['instrumentoJuridico'],
            "infterid" =>  $_POST['infterid'],
        );
        $file = new FilesSimec("documentoterritorio", $campos, 'sase');
        $file->setUpload($_FILES['arquivo']['name']);
    }
    die();
}

if ($_POST['filtroAjaxAddMunicipio']) {

    //remove o termo multiselect-all do array de municípios
    $posMultselect = array_search('multiselect-all', $_POST['muncod']);

    if($posMultselect  !== false){
        unset($_POST['muncod'][$posMultselect]);
    }

    ob_clean();

    foreach ($_POST['muncod'] as $muncod){
        $arrDadosMunicipio = array();
        $arrDadosMunicipio['muncod'] = $muncod;
        $arrDadosMunicipio['usucpf'] = $_SESSION['usucpf'];
        $arrDadosMunicipio['infterid'] = $_POST['infterid'];
        $Territoriomunicipio->salvarTerritorioMunicipio($arrDadosMunicipio);

    }
    echo tabelamunicipio();
    die();
}

if ($_POST['filtroAjaxDeletaMunicipio']) {
    ob_clean();


    $Territoriomunicipio->deleteTerritorioMunicipio();


    echo tabelamunicipio();
    die();
}


function tabelamunicipio(){

    global $db;

    $table = "<table class=\"table table-hover\" >
        <thead style=\"text-align: left\">
        <tr style=\"text-align: left\">
            <th style=\"text-align: left\">Ação</th>
            <th style=\"text-align: left\">UF</th>
            <th style=\"text-align: left\">Município</th>
        </tr>
        </thead>
        <tbody>";

    $sql = "SELECT
              ter.termunid,
              m.estuf,
              mundescricao
            FROM sase.territoriomunicipio ter
              INNER JOIN territorios.municipio m ON m.muncod = ter.muncod
            where ter.infterid ={$_GET['infterid']}";
    $arrTerritorioMunicipio = $db->carregar($sql);
    if(!empty($arrTerritorioMunicipio)){

        foreach ($arrTerritorioMunicipio as $territorioMunicipio){


            $table .= " <tr>
                                <td><button class=\"btn btn-danger\" type=\"button\" onclick=\"deletamunicipio(".$territorioMunicipio['termunid'].")\" ><span class=\"glyphicon glyphicon-trash\"></span></button></td>
                                <td>".$territorioMunicipio['estuf']."</td>
                                <td>".$territorioMunicipio['mundescricao']."</td>
                            </tr>";
        }
    }else{

        $table .= " <tr>
                <td  colspan=\"3\">
                    <div class=\"alert alert-info center\" style=\"text-align: center\">
                        Nenhum dado Encontrado!
                    </div>
                </td>
            </tr>";


    }

    $table .= " </tbody>
           </table>";


    return $table;


}

function showError(){
    echo "<script>
               $(function() {
                    $('#divErro').show();
                    setTimeout(function(){
                      $('#divErro').hide();
                    }, 5000)
                });
                </script>";
}


if ($_POST['formulario']) {
    unset($_POST['formulario']);

    // edicao
    if (!empty($_POST['infterid'])) {
        
        $sql = "SELECT count(*)
        FROM sase.informacoesterritorio
        WHERE orgid = '{$_POST['orgid']}' AND inftercor = '{$_POST['inftercor']}' AND infterid <> '{$_POST['infterid']}'";
        //dbg($sql,1);
        $retorna = $db->pegaUm($sql);
        if($retorna){
            echo 'A Cor já foi cadastradas em outro tipo de organização';
              showError(); die();
        }
        $_POST['contcpf'] = limpaCPF_CNPJ($_POST['contcpf']);
        $_POST['concep'] = limpaCPF_CNPJ($_POST['concep']);


        $retorno = $Informacoesterritorio->editarInformacoesterritorio();
        if(!$retorno){
            showError(); die();
        }


        $_POST['infterid'] = $retorno;

        // Existe informação de contato da organização
        if (!empty($_POST['conid'])){
            $retornoOrganizacao = $contatoOrganizacao->editarContatoOrganizacao();

            if(!$retornoOrganizacao){
                showError(); die();
            }

        }elseif(empty($_POST['conid']) && !empty($_POST['contcpf'])){
            $retornoOrganizacao = $contatoOrganizacao->salvarContatoOrganizacao();

            if(!$retornoOrganizacao){
                showError(); die();
            }
        }

        //Editar o órgão de educação de referencia
          if (!empty($_POST['torid'])){
            $_POST['tordesc'] = $_POST['tipoOrgaoSecretaria'];
            $_POST['entid'] = $_POST['seceducacaoreferencial'];
            $retornoTipoOrganizacao = $tipoOrgao->editarTipoOrganizacao();

            if(!$retornoTipoOrganizacao){
                showError(); die();
            }

        }elseif(empty($_POST['torid'])){
               if ($_POST['seceducacaoreferencial'] != 0){
            $_POST['tordesc'] = $_POST['tipoOrgaoSecretaria'];
            $_POST['entid'] = $_POST['seceducacaoreferencial'];
            $retornoTipoOrganizacao = $tipoOrgao->salvarTipoOrganizacao();

            if(!$retornoTipoOrganizacao){
                showError(); die();
            }
               }
        }
        
        echo "<script>
                bootbox.alert('Os dados foram salvos com sucesso!');
                window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&infterid='+" . $_POST['infterid'] . ";
              </script>";

    } else {

        $sql = "SELECT count(*)
        FROM sase.informacoesterritorio
        WHERE orgid = '{$_POST['orgid']}' AND inftercor = '{$_POST['inftercor']}'";
        //dbg($sql,1);
        $retorna = $db->pegaUm($sql);
        if($retorna){
              showError(); die();
        }
          if(strlen($_POST['tipoOrgaoSecretaria']) > 500){
           $_POST['tipoOrgaoSecretaria'] = substr($_POST['tipoOrgaoSecretaria'], 0, 500);  
         }
        $_POST['contcpf'] = limpaCPF_CNPJ($_POST['contcpf']);
        $_POST['concep'] = limpaCPF_CNPJ($_POST['concep']);


        $retorno = $Informacoesterritorio->SalvarInformacoesterritorio();
        if(!$retorno){
            showError(); die();
        }


        $_POST['infterid'] = $retorno;

        if (!empty($_POST['contcpf'])){
            $retornoOrganizacao = $contatoOrganizacao->salvarContatoOrganizacao();
            if (!$retornoOrganizacao) {
                showError(); die();
            }
        }
        if ($_POST['seceducacaoreferencial'] != 0){
            $_POST['tordesc'] = $_POST['tipoOrgaoSecretaria'];
            $_POST['entid'] = $_POST['seceducacaoreferencial'];
            $retornoOrgaoEducacao =  $tipoOrgao->salvarTipoOrganizacao();
            if(!$retornoOrgaoEducacao){
            showError(); die();
        }
            
        }
        echo "<script>
                 bootbox.alert('Cadastro realizado com sucesso!');
                 window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&infterid='+" . $_POST['infterid'] . ";
              </script>";
    }
}
// submit /

function limpaCPF_CNPJ($valor)
{
    $valor = trim($valor);
    $valor = str_replace(".", "", $valor);
    $valor = str_replace(",", "", $valor);
    $valor = str_replace("-", "", $valor);
    $valor = str_replace("/", "", $valor);
    return $valor;
}

if ($_GET['verificaOrgaoAjax']) {
    ob_clean();
    if ($_GET['mesid']) {
        $sql = "select orgid from sase.mesoregiao mes
        inner join sase.orgaoestado oe on oe.oesid = mes.oesid
        where mesid = {$_GET['mesid']}";
        $orgidnew = $db->pegaLinha($sql);
    }
    if ($_GET['terid']) {
        if ($_GET['orgidold'] != $orgidnew['orgid']) {
            echo json_encode(1);
        } else {
            echo json_encode(0);
        }
    } else {
        echo json_encode(0);
    }
    die();
}


if ($_GET['filtroAjaxSecreitaria']) {
    ob_clean();

    if ($_GET['secreitaria']) {
        $sql = "SELECT entnome , entemail,entnumcpfcnpj,entnumdddcomercial,entnumcomercial,entnumdddcelular,entnumcelular
         FROM entidade.entidade
        where entid =  {$_GET['secreitaria']}";
        $secreitaria = $db->carregar($sql);


        die(json_encode($secreitaria[0]));
    }
    die();
}
/**
 * lista Tipo de Organização
 */

$sqlorg = "SELECT  		DISTINCT org.orgid, orgdsc 
		   FROM 		sase.orgao org
           LEFT JOIN 	sase.orgaoestado oe on oe.orgid = org.orgid
		   WHERE		orgstatus = 'A'
           ORDER BY 	orgdsc ASC";

$rowOrgaos = $db->carregar($sqlorg);
/**
 * lista Secretaria de Educação
 */
$sql = "SELECT ent.entid,ent.entnome
              FROM entidade.entidade ent
                INNER JOIN entidade.funcaoentidade f on f.entid=ent.entid
                where funid in (6,7) ORDER BY entnome asc
";

$entidades = $db->carregar($sql);


/**
 * lista Documentos
 */
$arrDocumentos = array();
if (!empty($_GET['infterid'])) {
    $sql = "SELECT * from sase.documentoterritorio WHERE infterid ={$_GET['infterid']}";
    $arrDocumentos = $db->carregar($sql);

    $sql = "SELECT
              ter.termunid,
              m.estuf,
              mundescricao
            FROM sase.territoriomunicipio ter
              INNER JOIN territorios.municipio m ON m.muncod = ter.muncod
            where ter.infterid ={$_GET['infterid']}";
    $arrTerritorioMunicipio = $db->carregar($sql);


}




function inserirAnexoDetalhamento($dados)
{


    if ($_FILES['arquivo']['error'] == 0) {

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $campos = array(
            "taoid" => "'" . $dados['taoid'] . "'",
            "odsid" => "'" . $dados['odsid'] . "'",
            "aosdsc" => "'" . $dados['aosdsc_'] . "'",
            "aosdtinclusao" => "NOW()",
            "aosstatus" => "'A'"
        );


        $file = new FilesSimec("anexoordemservico", $campos, "fabrica");
        $file->setUpload((($dados['aosdsc_']) ? $dados['aosdsc_'] : null), $key = "arquivo");

        if ($dados['redirecionamento']) {

            echo "<script>
					alert('Arquivo anexado com sucesso');
					window.location='" . $dados['redirecionamento'] . "';
				  </script>";
        }
    }
}




//municipios
if( $_GET['filtroAjaxMunicipio'] ){

    $sql = "SELECT mun.muncod, mun.estuf, mun.mundescricao
            FROM territorios.municipio mun
            WHERE mun.muncod NOT IN (
                   SELECT muncod
                   FROM sase.territoriomunicipio
                   WHERE infterid =  '" . $_GET['infterid']. "' AND mun.estuf = '" . $_GET['filtroEstado'] . "'
                 ) AND mun.estuf = '" . $_GET['filtroEstado'] . "' ";
    $municipios = $db->carregar($sql);
    ob_clean(); ?>
    <select name="muncod" id="muncod"   class="form-control"  multiple="multiple">
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>

            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
    <?php die();
}


function instrumentoJuridico($tipo)
{
    if ($tipo == 1) {
        return 'Contrato de Consórcio Público';
    } elseif ($tipo == 2) {
        return 'Convênio Intermunicipal';
    } elseif ($tipo == 3) {
        return 'Termo, Acordo ou Protocolo de Colaboração Interfederativa';
    } elseif ($tipo == 4) {
        return 'Estatuto e Ata de Criação de Organização Não Governamental';
    } elseif ($tipo == 5) {
        return 'Plano de Ação de Território Etnoeducacional';
    } elseif ($tipo == 6) {
        return 'Portarias de Nomeação de Colegiado, Grupo de Trabalho, Comissão, Conselho ou Símile';
    } elseif ($tipo == 7) {
        return 'Outros';
    } else {
        return '';
    }
}

?>
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="/sase/js/spectrum-colorpicker/spectrum.js"></script>
<link rel="stylesheet" type="text/css" href="/sase/js/spectrum-colorpicker/spectrum.css">
<link rel="stylesheet" href="../library/chosen-1.0.0/chosen.css" media="screen">
<script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

<!-- js -->
<script>


    function verificacorexistente(cor) {
        var infterid = <?=(!empty($_GET['infterid']) ? $_GET['infterid'] : 0)?>;
        console.log(infterid);
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjaxcor: true,
                cor: cor,
                infterid: infterid
            },
            success: function (cor) {
                var arr = $.parseJSON(cor);
                if (arr > 0) {
                    $('#corCadastrada').show();
                    $('.sp-preview-inner').attr('style', 'background-color: rgb(0, 0, 0);');
                    setTimeout(function () {
                        $('#corCadastrada').fadeOut('fast');
                    }, 4000);
                    $('#cor').val('');
                } else {
                    $('#corCadastrada').hide();
                    $('.sp-replacer').attr('style', 'border: 1px solid #CCC;');
                }
               // verificaCorTipoOrganizacaoCadastrada();
            }
        });
    }

    function verificaMunicipioExistente(muncod) {
        var muncod = <?=(!empty($_GET['infterid']) ? $_GET['infterid'] : 0)?>;
        console.log(muncod);
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjaxTerritorioMunicipio: true,
                muncod: muncod
            },
            success: function (cor) {
                var arr = $.parseJSON(cor);
                if (arr > 0) {
//                    $('#corCadastrada').show();
//                    $('.sp-preview-inner').attr('style', 'background-color: rgb(0, 0, 0);');
//                    setTimeout(function () {
//                        $('#corCadastrada').fadeOut('fast');
//                    }, 4000);
//                    $('#cor')val('');
                    alert('Tetes jksadjfkalj ajf ')
                } else {
                    $('#corCadastrada').hide();
                    $('.sp-replacer').attr('style', 'border: 1px solid #CCC;');
                }
            }

        });
    }


    function filtraMunicipios(estuf) {


        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjaxMunicipio: true,
                filtroEstado: estuf
            },
            success: function (resposta) {
                jQuery('#divFiltroMunicipio').html(resposta);
                jQuery('#muncod').multiselect({
                    numberDisplayed: 1,
                    includeSelectAllOption: true,
                    enableCaseInsensitiveFiltering: true,
                    enableFiltering: true,
                    maxHeight: 300,
                    width:480,

                    id: 'muncod'
                });





            }
        });
    }
    /*
     function filtraOrgao(estuf) {
     jQuery.ajax({
     url: '',
     type: 'GET',
     data: {
     filtroAjaxOrgao: true,
     filtroEstado: estuf
     },
     success: function (resposta) {
     jQuery('#divFiltroOrgao .inputarea').html(resposta);
     }
     });
     }

     function filtraTerritorio(estuf, orgao) {
     jQuery.ajax({
     url: '',
     type: 'GET',
     data: {
     filtroAjaxTerritorio: true,
     filtroEstado: estuf,
     filtroOrgao: orgao
     },
     success: function (resposta) {
     jQuery('#divFiltroTerritorio .inputarea').html(resposta);
     }
     });
     }


     function salvar() {
     var mesid = $("#mesid").val();
     var orgidold = $("#orgidold").val();
     /!*alert(mesid + '------' + orgidold);
     return false;*!/
     jQuery.ajaselectx({
     url: '',
     type: 'GET',
     data: {
     verificaOrgaoAjax: true,
     orgidold: orgidold,
     mesid: mesid
     },
     success: function (resposta) {
     if (resposta == 1) {
     if (confirm("O Órgão da Mesoregião escolhida é diferente da atual. Deseja prosseguir mesmo assim?")) {
     jQuery('[name=formEdicaoTerritorio]').submit();
     }
     } else {
     jQuery('[name=formEdicaoTerritorio]').submit();
     }
     }
     });
     }*/
</script>
<!-- /js -->
<div id="container">
    <div class="alert alert-danger" id="divErro" style="display: none;">
        <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
        <strong>Erro!</strong> cadastrar Informações Territoriais
    </div>

    <ul class="nav nav-tabs">
        <li class="active"><a data-toggle="tab" href="#informacao">Informações Território</a></li>
        <li><a data-toggle="tab" href="#municipios" <?= ($orgTerId != "" ? "" : 'style="display: none"'); ?>
            ">Municípios Território</a></li>

    </ul>

    <div class="tab-content">
        <div id="informacao" class="tab-pane fade in active">
            <?php include_once 'inforamacoesTerritorial.inc' ?>
        </div>
        <div id="municipios" class="tab-pane fade">
            <?php include_once 'municipiosTerritorio.inc' ?>
        </div>

    </div>
</div>


<!-- / ------------------------------- FORM ------------------------------- -->

</div>

<div id="footer"></div>
<?php
if (!empty($arrDados['entid'])) {
    echo "<script> $(function ($) { buscaDadossecretaria(); });</script>";
}
?>

<!-- /html -->
<script>
    $(function () {
        $('#email').focusout(function () {
            var email = $('#email').val();
            if (email != "") {
                var regex = /^[a-zA-Z0-9.!#$%&'*+\/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/;
                var valido = regex.test(email);
                if (!valido) {
                    alert('Por favor, informe um email válido.');
                }
            }
        });

        $('#salvarOrganizacao').click(function () {
            var msg = [];
            $('#cpf').attr('style', 'border: 1px solid #CCC;');
            $('#nome').attr('style', 'border: 1px solid #CCC;');
            $('#cargofuncao').attr('style', 'border: 1px solid #CCC;');
            $('#telefone1').attr('style', 'border: 1px solid #CCC;');
            $('#secretaria').attr('style', 'border: 1px solid #CCC;');
            $('.sp-replacer').attr('style', 'border: 1px solid #CCC;');

            if(!$('#nomeOrganizacao').val()){
                msg.push('Nome da Organização é obrigatório');
                $('#nomeOrganizacao').attr('style', 'border: 1px solid #F00;');
            };
            
            if($('#tipoOrganizacao').val() == 0){
                msg.push('Tipo da Organização é obrigatório');
                $('#tipoOrganizacao').attr('style', 'border: 1px solid #F00;');
            };

//            if (!$('#cpf').val()) {
//                msg.push('Campo "CPF" é obrigatório');
//                $('#cpf').attr('style', 'border: 1px solid #F00;');
//            }
//            if (!$('#nome').val()) {
//                msg.push('Campo "Nome" é obrigatório');
//                $('#nome').attr('style', 'border: 1px solid #F00;');
//            }
//            if (!$('#cargofuncao').val()) {
//                msg.push('Campo "Cargo/Função" é obrigatório');
//                $('#cargofuncao').attr('style', 'border: 1px solid #F00;');
//            }
//            if (!$('#telefone1').val()) {
//                msg.push('Campo "Telefone 1" é obrigatório');
//                $('#telefone1').attr('style', 'border: 1px solid #F00;');
//            }
//            if ($('#secretaria').val() == 0) {
//                msg.push('Campo "Secretaria de Educação" é obrigatório');
//                $('#secretaria').attr('style', 'border: 1px solid #F00;');
//            }

            var infterid = <?=(!empty($_GET['infterid']) ? $_GET['infterid'] : 0)?>;

            if ($('#cor').val() == '' && infterid > 0) {
                msg.push('Campo "Cor" é obrigatório; ');
                $('.sp-replacer').attr('style', 'border: 1px solid #F00;');

            }



            if ($('#email').val() != '') {
                var regex = /^[a-zA-Z0-9.!#$%&'*+\/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/;
                var valido = regex.test($('#email').val());
                if (!valido) {
                    msg.push('Por favor, informe um email válido');
                }
            }

            if (msg.length > 0) {
                bootbox.alert('Os seguintes campos são obrigatórios e devem ser preenchidos:<br /><br /><ul><li>' + msg.join(';</li><li>') + '.</li></ul>');
                return false;
            }

            $('[name=formEdicaoTerritorio]').submit();


        });


        $('#cor').spectrum({
            showPaletteOnly: true,
            togglePaletteOnly: true,
            preferredFormat: "hex",
            showInput: true,
            color: '<?=$cor?>',
            change: function (color) {
                verificaCorTipoOrganizacaoCadastrada();
                //verificacorexistente(color.toHexString());
            },
            palette: [
                ["#000", "#444", "#666", "#999", "#ccc", "#eee", "#f3f3f3", "#fff"],
                ["#f00", "#f90", "#ff0", "#0f0", "#0ff", "#00f", "#90f", "#f0f"],
                ["#f4cccc", "#fce5cd", "#fff2cc", "#d9ead3", "#d0e0e3", "#cfe2f3", "#d9d2e9", "#ead1dc"],
                ["#ea9999", "#f9cb9c", "#ffe599", "#b6d7a8", "#a2c4c9", "#9fc5e8", "#b4a7d6", "#d5a6bd"],
                ["#e06666", "#f6b26b", "#ffd966", "#93c47d", "#76a5af", "#6fa8dc", "#8e7cc3", "#c27ba0"],
                ["#c00", "#e69138", "#f1c232", "#6aa84f", "#45818e", "#3d85c6", "#674ea7", "#a64d79"],
                ["#900", "#b45f06", "#bf9000", "#38761d", "#134f5c", "#0b5394", "#351c75", "#741b47"],
                ["#600", "#783f04", "#7f6000", "#274e13", "#0c343d", "#073763", "#20124d", "#4c1130"]
            ]

        });

        $('#cpf').mask('999.999.999-99');
        $('#cep').mask('99999-999');
        $('#telefone1').mask('(99) 9999-99999');
        $('#telefone2').mask('(99) 9999-99999');

        $('#cpf').blur(function () {
            buscarUsuarioReceitaFederal($(this).val());
        });

        $('#cor').val('<?=$cor?>');
        //Quando o campo cep perde o foco.
        $("#cep").blur(function () {

            //Nova variável "cep" somente com dígitos.
            var cep = $(this).val().replace(/\D/g, '');

            //Verifica se campo cep possui valor informado.
            if (cep != "") {

                //Expressão regular para validar o CEP.
                var validacep = /^[0-9]{8}$/;

                //Valida o formato do CEP.
                if (validacep.test(cep)) {

                    //Preenche os campos com "..." enquanto consulta webservice.
                    $("#rua").val("...");
                    $("#bairro").val("...");
                    $("#cidade").val("...");
                    $("#uf").val("...");
                    $("#ibge").val("...");

                    //Consulta o webservice viacep.com.br/
                    $.getJSON("//viacep.com.br/ws/" + cep + "/json/?callback=?", function (dados) {

                        //console.log(dados)
                        if (!("erro" in dados)) {
                            //Atualiza os campos com os valores da consulta.
                            $("#logradouro").val(dados.logradouro);
                            $("#bairro").val(dados.bairro);
                            $("#cidade").val(dados.localidade);
                            $("#estado").val(dados.uf);

                        } //end if.
                        else {
                            //CEP pesquisado não foi encontrado.
                            limpa_formulário_cep();
                            alert("CEP não encontrado.");
                        }
                    });
                } //end if.
                else {
                    //cep é inválido.
                    limpa_formulário_cep();
                    alert("Formato de CEP inválido.");
                }
            } //end if.
            else {
                //cep sem valor, limpa formulário.
                limpa_formulário_cep();
            }
        });


        $('#addDocumento').click(function () {


            var arquivo = $("#arquivo").val();
            var instrumentoJuridico = $("#instrumentoJuridico").val();
            if (instrumentoJuridico == 0) {
                alert("Selecionar um Instrumento Jurídico!");
                return false;
            }

            if (arquivo != '') {
                var valido = validoFormatoArquivo(arquivo);
                if (valido == 0) {
                    alert("Selecionar arquivo do tipo PDF, Doc, Docx!");
                    return false;
                }
            } else {
                alert("Selecione um arquivo do tipo PDF, Doc, Docx!");
                return false;
            }

            var infter = <?=(!empty($_GET['infterid']) ? $_GET['infterid'] : 0)?>;
            var form_data = new FormData();
            form_data.append('arquivo', $("#arquivo").prop('files')[0]);
            form_data.append('infterid', infter);
            form_data.append('instrumentoJuridico', instrumentoJuridico);
            form_data.append('filtroAjaxDocumento', 'true');

            jQuery.ajax({
                url: '',
                type: 'POST',
                dataType: 'text',  // what to expect back from the PHP script, if anything
                contentType: false,
                processData: false,
                data: form_data,

                success: function (dados) {

                    alert('Documento cadastrado com sucesso');

                    location.href = 'sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&infterid=<?=$_GET['infterid']?>';


                }
            });
        });



        $('#btnAddMunicipio').click(function () {

            var infterid = <?=(!empty($_GET['infterid']) ? $_GET['infterid'] : 0)?>;
            var muncod = $('#muncod').val();
            var estuf = $('#estuf').val();
            if (estuf == 0){
                alert('Selecione um Estado');
                return false;
            }
            if (muncod == null){
                alert('Selecione um Município');
                return false;
            }

            jQuery.ajax({
                url: '',
                type: 'POST',
                data: {
                    filtroAjaxAddMunicipio: true,
                    muncod: muncod,
                    infterid:infterid
                },
                success: function (resposta) {
                    jQuery('#divmunicipiocadastrados').html(resposta);
                },
            });

        });



    });


    function deletamunicipio(termunid) {


        jQuery.ajax({
            url: '',
            type: 'POST',
            data: {
                filtroAjaxDeletaMunicipio: true,
                termunid: termunid
            },
            success: function (resposta) {
                alert('Municipio Excluído com sucesso!')
                jQuery('#divmunicipiocadastrados').html(resposta);
            }
        });
    }

    function validoFormatoArquivo(arquivo) {
        var extensoes, ext, valido;
        extensoes = new Array('.pdf', '.docx', '.doc');
        ext = arquivo.substring(arquivo.lastIndexOf(".")).toLowerCase();
        valido = false;
        for (var i = 0; i <= arquivo.length; i++) {
            if (extensoes[i] == ext) {
                valido = true;
                break;
            }
        }
        if (valido) {
            return 1;
        }
        return 0;
    }


    function buscaDadossecretaria() {
        var secretaria = $('#secretaria').val();

        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjaxSecreitaria: true,
                secreitaria: secretaria

            },
            success: function (dados) {
                //console.log(dados);
                var arr = $.parseJSON(dados);
                //console.log(arr);


                $('#nomeSecretaria').val(arr.entnome);
                $('#cpfSecretaria').val(arr.entnumcpfcnpj);
                $('#emailSecretaria').val(arr.entemail);
                $('#telefone1Secretaria').val(arr.entnumdddcomercial + '-' + arr.entnumcomercial);
                if (arr.entnumdddcelular == null) {
                    arr.entnumdddcelular = '';
                }
                if (arr.entnumcelular == null) {
                    arr.entnumcelular = '';
                }

                $('#telefone2Secretaria').val(arr.entnumdddcelular + '-' + arr.entnumcelular);


            }
        });

    }


    function limpa_formulário_cep() {
        // Limpa valores do formulário de cep.
        //Atualiza os campos com os valores da consulta.
        $("#logradouro").val("");
        $("#cep").val("");
        $("#bairro").val("");
        $("#cidade").val("");
        $("#estado").val("");
    }

    function divcontatoorganizacao() {

        var nomeOrganizacao = $('#nomeOrganizacao').val();
        var tipoOrganizacao = $('#tipoOrganizacao').val();
        var status = $('#status').val();
        $('#nomeOrganizacao').attr('style', 'border: 1px solid #CCC;');
        $('#tipoOrganizacao').attr('style', 'border: 1px solid #CCC;');
        $('#status').attr('style', 'border: 1px solid #CCC;');
        //console.log(nomeOrganizacao);
        if (nomeOrganizacao == '') {
            $('#nomeOrganizacao').attr('style', 'border: 1px solid #F00;');
            return false
        }
        if (tipoOrganizacao == 0) {
            $('#tipoOrganizacao').attr('style', 'border: 1px solid #F00;');
            // $('#nomeOrganizacao').focus();
            return false
        }
        if (status == 0) {
            $('#status').attr('style', 'border: 1px solid #F00;');
            //$('#status').focus();
            return false
        }

        $('#divConatatoOrganizacao').show(1000);
        $('#divSecretaria').show(1000);
        $('#salvarOrganizacao').prop('disabled', false);

    }
    
    function verificaCorTipoOrganizacaoCadastrada(){
 
            //alert(jQuery('#cor').val()+' '+jQuery('#tipoOrganizacao').val());
            var cor = jQuery('#cor').val();
            var orgid = jQuery('#tipoOrganizacao').val();
            
            jQuery.ajax({
            url: '',
            type: 'GET',
            data: {
                filtroAjaxcorOrganizacao: true,
                cor: cor,
                orgid: orgid
            },
            success: function (cor) {
                //alert(cor);
             if(cor != false){
                 alert("O tipo de organização já possui cadastro de território com a cor selecionada");
                 $('#tipoOrganizacao').val(0);
             }
            }
        });
    
    }
    
    function buscarUsuarioReceitaFederal() {

        var cpf = $('#cpf').val();

        cpf = cpf.replace('-', '');
        cpf = cpf.replace('.', '');
        cpf = cpf.replace('.', '');


        $.ajax({
            url: '/includes/webservice/cpf.php',
            type: 'post',
            data: {ajaxCPF: cpf},
            dataType: "html",
            success: function (data) {
                //console.log(data);
                pessoaFisica = formatarDadosPessoaFisicaReceita(data);
                //console.log(pessoaFisica);
                if (typeof(pessoaFisica['no_pessoa_rf']) == "undefined") {
                    alert('O CPF informado não foi encontrado na base de dados da Receita federal.');
                    return false;
                } else {
                    $('#nome').val(pessoaFisica['no_pessoa_rf']);
                    $('#cpf').attr('style', 'border: 1px solid #CCC;');
                    $('#nome').attr('style', 'border: 1px solid #CCC;');


                }
            }

        });

    }


    function formatarDadosPessoaFisicaReceita(text) {
        var pessoaFisica = [];
        linhaItem = text.split('|');
        for (i = 0; i < linhaItem.length; i++) {
            colunaItem = linhaItem[i].split('#');
            pessoaFisica[colunaItem[0]] = colunaItem[1];
        }

        return pessoaFisica;
    }


</script>