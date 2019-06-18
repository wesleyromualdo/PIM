<?php
include_once 'programas/_funcoes_maismedicos.php';

if ($_REQUEST['requisicao'] == 'alterarInfor') {
    alterarInfor($_POST);
    exit();
}

if ($_REQUEST['requisicao'] == 'salvarTermoResidencia') {
    salvarTermoResidencia($_POST);
    exit();
}

if ($_REQUEST['prgid']) {
    $modelProadesao = new Par3_Model_Proadesao();
    $proadesaoDados = $modelProadesao->pegarAdesaoPorPrograma($_REQUEST['prgid']);
    $dtFinal = (int)str_replace('-', '', $proadesaoDados['pfadatafinal']);
    $dtAtual = (int)str_replace('-', '', date('Y-m-d'));
    $somenteLeitura = ($dtFinal > 0 && ($dtAtual > $dtFinal)) && !$boolAbaAnexo ? true : false;
}


#pega o nome do municipio
$municipio = $controleUnidade->pegarMunicipio($_REQUEST['inuid']);
$muncod = $controleUnidade->pegarMuncod($_REQUEST['inuid']);

$estado = $db->pegaUm("select estuf from territorios.municipio  where muncod = '{$muncod}'");

?>

<style>
    .titulo {
        font-size: 15px;
        font-family: arial;
    }

    .tituloJustificado {
        text-align: justify;
        text-justify: inter-word;
    }

    .red {
        background-color: red;
    }
</style>


<script type="text/javascript" language="javascript">
    jQuery(document).ready(function () {

    });

    function abrirTermo(pmmid) {
        return window.open('par.php?modulo=principal/programas/feirao_programas/maisMedicosTermoParceria&acao=A&pmmid=' + pmmid, 'modelo', "height=600,width=950,scrollbars=yes,top=50,left=200");
    }

    function salvarTermoResidencia() {

        jQuery.ajax({
            url: 'par3.php?modulo=principal/adesao/termos/mais_medicos_2017/maisMedicosInforMunicipio&acao=A',
            data: {
                requisicao: 'salvarTermoResidencia',
                prgid: jQuery("#prgidMunicipio").val(),
                muncod: jQuery("#muncodMunicipio").val()
            },
            async: false,
            type: 'POST',
            success: function (data) {
                if (trim(data) == 'S') {
                    $("#termoresidencia").hide();
                    $("#termoresidenciaAceito").show();
                    alert('Termo de Compromisso Residência Médica aceito com sucesso!');
                    setTimeout(function(){ window.location='par3.php?modulo=principal/adesao/termo&acao=A&prgid=<?php echo $_REQUEST['prgid']; ?>&inuid=<?php echo $_REQUEST['inuid']; ?>&aba=termo'; }, 3000);


                } else {
                    alert('Não foi possível aceitar o Termo de Compromisso Residência Médica!');
                }
            }
        });
    }

</script>

<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <left><h3> TERMO DE COMPROMISSO PARA OFERTA DE RESIDÊNCIA MÉDICA - <?php echo $municipio; ?></h3></left>
            </div>
        </div>
    </div>

    <div class="ibox-content">
        <form action="" method="POST" id="formularioAceite" name="formularioAceite" enctype="multipart/form-data">
            <input type="hidden" id="requisicao" name="requisicao" value="alterarInfor"/>
            <input type="hidden" id="muncodMunicipio" name="muncodMunicipio" value="<?php echo $muncod; ?>"/>
            <input type="hidden" id="prgidMunicipio" name="prgidMunicipio" value="<?php echo $_REQUEST['prgid']; ?>"/>

            <div class="row">
                <div class="center titulo col-md-12">

                    <!--</div>-->
                    <div class="col-md-1"></div>
                    <div class="col-md-10" style="text-align: left">
                        <p>
                            O Município de <b><?= $municipio; ?></b> / <b><?= $estado; ?></b> se compromete, juntamente
                            com a Instituição de Educação
                            Superior privada vencedora do chamamento público para seleção de propostas de mantenedoras
                            para autorização de funcionamento de cursos de Medicina, nos termos da Lei nº 12.871, de 22
                            de outubro de 2013, a apoiar a oferta e o desenvolvimento de Programas de Residência Médica
                            em seu território de modo a que, até 1 (um) ano após o início das atividades do curso de
                            Medicina, no mínimo 03 (três) Programas de Residência Médica estejam implementados nas áreas
                            prioritárias referidas no art. 6º, incisos I e II da Lei nº 12.871 de 2013,
                            preferencialmente em Medicina Geral de Família e Comunidade.
                        </p>
                        <p>
                                
                        </p>
                    </div>
                    <div class="col-md-1"></div>
                </div>
            </div>
        </form>
    </div>
    <div class="ibox-footer center">
        <?php
            if ($termoAceite == '') {?>
                    <button type="button" name="termoresidencia" id="termoresidencia" value="Aceito"
                            onclick="salvarTermoResidencia();" class="btn btn-success center">
                        Aceito
                    </button>


           <?php } else {
             $adesao = $db->pegaUm("SELECT tcraceite FROM maismedicomec.termocompromissoresidencia WHERE tcrid = ".$termoAceite);

        if($adesao == 'A') {
            ?>
            <button type="button" name="termoresidenciaAceito" id="termoresidenciaAceito" value="Aceito" class="btn btn-info center" disabled>
                Termo aceito
            </button>
        <?php
            }else{        ?>
            <button type="button"  value="Não aceito" class="btn btn-danger center" disabled>
                Não aceito
            </button>
        <?php }} ?>
    </div>
</div>
<br>
<br>
