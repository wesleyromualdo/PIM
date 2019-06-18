<?php
include_once('configuracao.php');
include_once('cabecalho.inc');

$dados = [];

if ($_SERVER['REQUEST_METHOD'] === 'POST') $dados = $_POST;
if ($_SERVER['REQUEST_METHOD'] === 'GET') $dados = $_GET;


if (array_key_exists('estuf', $dados)) {
    $dados['uf'] = $dados['estuf'];
    $dados['regcod'] = $transparenciaPublicaItens->buscaRegiaoPorEstado($dados['estuf']);
    unset($dados['estuf']);
}

switch ($dados['acao']) {
    case 'carregarMunicipios':
        ob_clean();
        $sqlMunicipios = sprintf(TransparenciaPublicaItens::$sqlMunicipios, $dados['uf']);
        die($simec->select('muncod', 'Município', $dados['muncod'] ?: null, $sqlMunicipios, [], ['tempocache' => 86400]));
        break;
    case 'carregarEstados':
        ob_clean();
        $whereStr = '';
        if (!empty($dados['regcod'])) {
            $whereStr = " WHERE regcod = '{$dados['regcod']}' ";
        }
        $sqlEstados = sprintf(TransparenciaPublicaItens::$sqlEstados, $whereStr);
        die($simec->select('uf', 'UF', $dados['uf'] ?: null, $sqlEstados, [], ['tempocache' => 86400]));
        break;
}

if (array_key_exists('sql', $_SESSION)) {
    unset($_SESSION['sql']);
}

?>
    <div class="ibox">
        <div class="ibox-content">
            <div class="row" style="border: 1px solid white;">
                <div class="col-lg-12 col-md-12">
                    <div class="space-20"></div>
                    <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                        <div class="form-group">
                            <div class="row">
                                <div class="col-md-6">
                                    <?php echo $simec->radio('itrid', 'Esfera', $dados['itrid'] ?: 'T', ['T' => 'Todos', '1' => 'Estadual', '2' => 'Municipal', '3' => 'Federal'], ['required']); ?>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-md-6">
                                    <?php echo $simec->select('regcod', 'Região', $dados['regcod'] ?: null, TransparenciaPublicaItens::$sqlRegioes, [], ['tempocache' => 86400]); ?>
                                </div>
                                <div class="col-md-6">
                                    <?php echo $simec->select('intoid', 'Tipo Objeto', $dados['intoid'] ?: null, TransparenciaPublicaItens::$sqlIniciativaTipoObjeto, [], ['tempocache' => 86400]); ?>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-md-6">
                                    <div id="div_estados">
                                        <?php
                                        $whereStr = '';

                                        if (array_key_exists('regcod', $dados) && !empty($dados['regcod'])) {
                                            $whereStr = "WHERE regcod = '{$dados['regcod']}'";
                                        }

                                        $sqlEstados = sprintf(TransparenciaPublicaItens::$sqlEstados, $whereStr);
                                        echo $simec->select('uf', 'UF', $dados['uf'], $sqlEstados, [], ['tempocache' => 86400]);
                                        ?>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-md-6">
                                    <div id="div_municipios">
                                        <?php if (array_key_exists('muncod', $dados) && !empty($dados['muncod'])) : ?>
                                        <?php $sqlMunicipios = sprintf(TransparenciaPublicaItens::$sqlMunicipios, $dados['uf']); ?>
                                        <?php echo $simec->select('muncod', 'Município', $dados['muncod'], $sqlMunicipios, [], ['tempocache' => 86400]); ?>
                                        <?php endif ?>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-md-6">
                                    <?php echo $simec->select('etaid', 'Etapa Ensino', $dados['etaid'] ?: null, TransparenciaPublicaItens::$sqlEnsinoEtapa, [], ['tempocache' => 86400]); ?>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="row">
                                <div class="col-md-6">
                                    <?php echo $simec->select('modid', 'Modalidade', $dados['modid'] ?: null, TransparenciaPublicaItens::$sqlModalidade, [], ['tempocache' => 86400]); ?>
                                </div>
                            </div>
                        </div>

                        <div class="form-group">
                            <div class="col-lg-12 col-md-12 text-center">
                                <button type="button" class="btn btn-sm btn-success btn-lg btnPesquisar">
                                    <span class="glyphicon glyphicon-search"> </span> Pesquisar
                                </button>

                                <button type="button" class="btn btn-sm btn-success btn-lg btnLimpar">
                                    <span class="glyphicon glyphicon-trash"> </span> Limpar
                                </button>
                            </div>
                        </div>
                    </form>
                </div>
            </div>
        </div>
    </div>

    <?php if ($_SERVER['REQUEST_METHOD'] === 'POST') : ?>
        <div class="ibox">
            <div class="ibox-content">
                <div class="row" style="border: 1px solid white;">
                    <div class="col-lg-12 col-md-12">
                        <div class="space-20"></div>
                        <?php $transparenciaPublicaItens->montarListagemTransparenciaV1($dados); ?>
                    </div>
                </div>
            </div>
        </div>
    <?php endif; ?>

    <script type="text/javascript">
        function loadMunicipios(uf) {
            $('#div_municipios').load('?acao=carregarMunicipios&uf=' + uf);
        }

        function loadEstados(regcod) {
            $('#div_estados').load('?acao=carregarEstados&regcod=' + regcod, function () {
                $('#uf').change(function () {
                    if ($('[name="itrid"]:checked').val() === '2') {
                        loadMunicipios($('#uf').val());
                    }
                });
            });
        }

        $(function () {
            $('#regcod').change(function () {
                loadEstados($(this).val());
            });

            $('#uf').change(function () {
                if ($('[name="itrid"]:checked').val() === '2') {
                    loadMunicipios($('#uf').val());
                }
            });

            $('[name="itrid"]').change(function () {
                if ($('[name="itrid"]:checked').val() === '2' && $('#uf').val() != '') {
                    loadMunicipios($('#uf').val());
                } else {
                    $('#div_municipios').html('');
                }
            });

            $('.btnLimpar').click(function () {
                $(".chosen-select").val('').trigger("chosen:updated");
                $('#div_estados').html('');
                $('#div_municipios').html('');
            });

            $(".btnPesquisar").click(function () {
                $('#formulario').submit();
            });
        });
    </script>

<?php
unset($_SESSION['sislayoutbootstrap']);
include_once('rodape.inc');