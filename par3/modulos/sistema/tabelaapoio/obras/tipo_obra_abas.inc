<?php
switch ($_REQUEST['req']) {
    default:
        break;
}
$url = 'par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=tipo_obra_abas&';
$abasQuestoes= array(
    0 => array( "descricao" => "Tipo de Obra - Lista",      "link" => "par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=listar_tipo_obra&"),
    1 => array( "descricao" => "Tipo de Obra - Cadastro",   "link" => "par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=tipo_obra&"),
    2 => array( "descricao" => "Tipo de Obra - Abas",       "link" => "par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=tipo_obra_abas&"),
    3 => array( "descricao" => "Tipo de Obra - Planilhas",  "link" => "par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=tipo_obra_planilhas&"),
);
echo $simec->tab ( $abasQuestoes , $url );
?>
<div class="ibox" id="painelProgramas">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h3 class="center">Tipo de Obra - Abas</h3>
            </div>
        </div>
    </div>
    <div class="ibox-content">
        <div class="row">
            <div class="col-md-offset-1 col-md-10 center">
                <table class="table">
                    <thead>
                        <tr>
                            <td></td>
                            <td>Aba</td>
                            <td>Configurar</td>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Aba Relatório de Vistoria</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Cadastro de Fotos do Terreno</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Cronograma</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Cronograma Contrapartida</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Documentos Anexos</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Enviar para Análise</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Análise de Engenharia</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Obras no Município</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Dados Orçamentários</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                        <tr>
                            <td>
                                <div class="checkbox m-r-xs">
                                    <input type="checkbox" id="checkbox2"><label for="checkbox2">&nbsp;</label>
                                </div>
                            </td>
                            <td>Documentos FNDE</td>
                            <td><i class="fa fa-search" aria-hidden="true" style="cursor:pointer"></i></td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <div class="ibox-footer">
            <div class="center">
                <button type="button" class="btn btn-success" id="btn-incluir" url="window.location.href=par3.php?modulo=sistema/tabelaapoio/guiaObras&acao=A&menu=tipo">
                    <i class="fa fa-save"></i> Salvar
                </button>
            </div>
        </div>
    </div>
</div>