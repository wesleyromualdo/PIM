<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 07/06/2016
 * Time: 10:24
 */
global $db;
$pfls = arrayPerfil();
/**
 * Carrega os estados
 */
$sql = "select estuf, estdescricao from territorios.estado order by estuf";
$estados = $db->carregar($sql);

$esp = new Sase_Model_Espacoparticipacao();
if($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'pesquisar':
            ob_clean();
            $estuf = $_REQUEST['estuf'];
            $muncod = $_REQUEST['muncod'];
            $esp = new Sase_Model_Espacoparticipacao();
            echo $esp->getTabelaLista($estuf, $muncod);
            exit;

        case 'carrega_municipio':
            ob_clean();
            $estuf = $_REQUEST['estuf'];
            $sql = "select muncod, mundescricao from territorios.municipio where estuf = '{$estuf}' order by mundescricao";
            $res = $db->carregar($sql);
            $options = "";
            foreach ($res as $r) {
                $options .= "<option value=\"{$r['muncod']}\">{$r['mundescricao']}</option>";
            }

            $html = <<<HTML
                <select name="muncod" id="muncod" class="form-control chosen-select">
                    <option value="">Selecione</option>
                    {$options}
                </select>
HTML;
            echo $html;
            exit;
    }
}

?>
<script>
    function pesquisar(){
        estuf = $('#estuf').val();
        muncod = $('#muncod').val();
        $.ajax({
            url: '',
            type: 'POST',
            data:{
                estuf: estuf,
                muncod: muncod,
                acao: 'pesquisar'
            },
            success: function(e){
                $('#lista').html(e);
            }
        });
    }

    function abreCadastro(espid){
        window.location.href = "/sase/sase.php?modulo=principal/espacodeparticipacao&acao=A&tipo=municipio&aba=edicao&espid="+espid;
    }
</script>
<div id="formFiltros">
    <form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>

        <div class="row" style="margin-top: 10px">
            <div class="col-lg-offset-3 col-lg-6 col-lg-offset-2">
                <div class="well">

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Estado:</label>
                        <div class="col-lg-9">
                            <select name="estuf" id="estuf" class="form-control chosen-select">
                                <option value="">Selecione</option>
                                <?php
                                foreach ($estados as $chave => $estado) {
                                    echo <<<HTML
                                        <option value="{$estado['estuf']}">{$estado['estdescricao']}</option>
HTML;
                                }
                                ?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label for="estuf" class="col-lg-2 control-label">Municipio:</label>
                        <div class="col-lg-9" id="divMunicipio">
                            <select name="muncod" id="muncod" class="form-control chosen-select" disabled>
                                <option value="">Selecione</option>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-lg-2 control-label">&nbsp;</label>
                        <div class="col-lg-9">
                            <input type="button" class="btn btn-primary" onclick="pesquisar()" value="Pesquisar" />
                        </div>
                    </div>

                </div>
            </div>
        </div>
    </form>
    <div class="col-lg-offset-2 col-lg-8" id="lista">
        <?php echo $esp->getTabelaLista(); ?>
    </div>
</div>
<!-- EVENTOS -->
<script>
    $('#estuf').on('change', function(){
        $.ajax({
            url: '',
            type: 'POST',
            data:{
                acao: 'carrega_municipio',
                estuf: $(this).val()
            },
            success: function(data){
                $('#divMunicipio').html(data);
            }
        });
    });
</script>