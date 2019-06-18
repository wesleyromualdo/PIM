<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:08
 */
global $db;
$planodecarreira = new Planodecarreira();
$filtros         = array();
$pfls            = arrayPerfil();

$arrPermissao = $planodecarreira->retornaResponsabilidades();

// Filtra os municipios
$sql = "select * from territorios.municipio where 1=1";
if( $_REQUEST['estuf'] ) $sql .= " and estuf = '" . $_REQUEST['estuf'] . "' ";
if(is_array($arrPermissao['muncod'])){
    $sql .= " and muncod in ('".implode("','",$arrPermissao['muncod'])."') ";
}
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);


if( $_GET['filtroAjax'] ){
    ob_clean(); ?>
    <select name="muncod" class="form-control chosen-select" id="muncod">
        <option value="">Filtrar município:</option>
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>
            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
    <script type="text/javascript">$('#muncod').chosen();</script>
<?php die();}


?>
<script>
    function filtraMunicipios( estuf ){
        jQuery.ajax({
            url:'',
            type:'GET',
            data:{filtroAjax:true,estuf:estuf},
            success: function( resposta ){
                jQuery('#divFiltroMunicipio .inputarea').html( resposta );
            }
        });
    }

    function pesquisar(){
        console.log($('#muncod').val());
        jQuery('#acao').val('pesquisar');
        jQuery('[name=\'formPesquisaLista\']').submit()
    }


    function gerarXLS(){
//        alert('teste');
        $('#formulario').val('1');
        $('#funcao').val('xls');
        $('#formPesquisaLista').submit();
    }



    function editarPlanodeCarreira(pcpid){
        window.location.href = "/sase/sase.php?modulo=principal/planodecarreira&acao=A&aba=edicao&pcpid="+pcpid;
    }
</script>
<div id="formularioFiltros">
    <form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>
        <input id="formulario" name="formulario" value="" type="hidden"/>
        <input type="hidden" name="funcao" id="funcao" value=""/>

        <div class="row">
            <div class="col-lg-offset-3 col-lg-6 col-lg-offset-2">
                <div class="well">
                    <div class="form-group" id="divFiltroEstado">
                        <label for="filtroEstado" class="col-lg-2 control-label">Estado:</label>
                        <div class="col-lg-9">
                            <?php
                            $where = "";
                            if(is_array($arrPermissao['estuf'])){
                                $where = "where estuf in ('".implode("','",$arrPermissao['estuf'])."')";
                            }
                            ?>
                            <select name="estuf" id="estuf" class="form-control chosen-select" onchange="javascript:if(this.value!=''){filtraMunicipios(this.value);}">
                                <option value="">Filtrar estado</option>
                                <?php
                                $sql = "select estuf, estdescricao from territorios.estado {$where} order by estdescricao ASC";
                                $estados = $db->carregar($sql);
                                if(is_array($estados)){
                                foreach( $estados as $chave => $estado ){ ?>
                                    <option <?=($estado['estuf']==$_POST['estuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                <?php }}?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroMunicipio">
                        <label for="filtroMunicipio" class="col-lg-2 control-label">Município:</label>
                        <div class="col-lg-9 inputarea">
                            <?php if(!$_POST['estuf']){ echo "Selecione um Estado"; }else{ ?>
                                <select name="muncod" class="form-control chosen-select" id="muncod">
                                    <option value="">Filtrar município</option>
                                    <?php
                                    foreach( $municipios as $chave => $municipio ){ ?>
                                        <option <?=($municipio['muncod']==$_POST['muncod'])?'selected':''?> value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
                                    <?php }?>
                                </select>
                            <?php } ?>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroSituacao">
                        <label for="filtroSituacao" class="col-lg-2 control-label">Plano de Carreira:</label>
                        <div class="col-lg-9">
                            <select name="esdid" class="form-control chosen-select" id="esdid">
                                <option value="">Filtrar situação</option>
                                <?php
                                $sql = "select esdid, spcdsc from sase.sitplancarprofessor where spcstatus = 'A' order by spcid ASC ";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$_POST['esdid'])?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spcdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroSituacao">
                        <label for="filtroSituacao" class="col-lg-2 control-label">Adequação:</label>
                        <div class="col-lg-9">
                            <select name="esdid2" class="form-control chosen-select" id="esdid2">
                                <option value="">Filtrar situação</option>
                                <?php
                                $sql = "select esdid, spmdsc from sase.sitplanomunicipio where spcstatus = 'A' order by spmid ASC ";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$_POST['esdid2'])?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spmdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>
                    <div class="form-group" id="divFiltroPcr">
                        <label for="filtroPcr" class="col-lg-2 control-label">PCR:</label>
                        <div class="col-lg-9">
                            <select name="pcppcr" class="form-control chosen-select" id="filtroPcr">
                                <option value="" <?=empty($_REQUEST['pcppcr'])?'selected':''?>>Todos</option>
                                <option value="t" <?=('t' == $_REQUEST['pcppcr'])?'selected':''?>>Sim</option>
                                <option value="f" <?=('f' == $_REQUEST['pcppcr'])?'selected':''?>>Não</option>
                            </select>
                        </div>
                    </div>
                    <div class="form-group" id="divFiltroAdesao">
                        <label for="filtroAdesao" class="col-lg-2 control-label">Termo de Adesão:</label>
                        <div class="col-lg-9">
                            <select name="pcpadesao" class="form-control chosen-select" id="filtroAdesao">
                                <option value="" <?=empty($_REQUEST['pcpadesao'])?'selected':''?>>Todos</option>
                                <option value="t" <?=('t' == $_REQUEST['pcpadesao'])?'selected':''?>>Sim</option>
                                <option value="f" <?=('f' == $_REQUEST['pcpadesao'])?'selected':''?>>Não</option>
                            </select>
                        </div>
                    </div>
                    <div class="form-group" id="divFiltroAto">
                        <label for="filtroAto" class="col-lg-2 control-label">Ato Legal da Comissão:</label>
                        <div class="col-lg-9">
                            <select name="pcpato" class="form-control chosen-select" id="filtroAto">
                                <option value="" <?=empty($_REQUEST['pcpato'])?'selected':''?>>Todos</option>
                                <option value="t" <?=('t' == $_REQUEST['pcpato'])?'selected':''?>>Sim</option>
                                <option value="f" <?=('f' == $_REQUEST['pcpato'])?'selected':''?>>Não</option>
                            </select>
                        </div>
                    </div>
                    <div class="form-group">
                        <label class="col-lg-2 control-label">&nbsp;</label>
                        <div class="col-lg-9">
                            <input type="button" class="btn btn-primary" onclick="pesquisar()" value="Pesquisar" />
                            &nbsp;&nbsp;
                            <input type="button" class="btn btn-primary" onclick="jQuery('[name=formulario]').val('1'); window.location.href='sase.php?modulo=principal/planodecarreira&acao=A&aba=lista'" value="Listar Todos" />
                            &nbsp;&nbsp;
                            <input type="button" class="btn btn-primary" onclick="gerarXLS()" value="Exportar XLS" />
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <hr/>

    </form>
    <div id="lista">
        <?php $planodecarreira->getLista(); ?>
    </div>
</div>