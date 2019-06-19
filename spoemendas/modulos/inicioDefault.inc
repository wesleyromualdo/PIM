<style>
    .vermelho {
        color: #228B22;
    }
</style>

<link href="/includes/spo.css" rel="stylesheet" media="screen">
<link href="/library/bootstrap-animate/animate.css" rel="stylesheet" media="screen">
<br/>
<table width="95%" class="tabela" bgcolor="#f5f5f5" border="0" cellSpacing="1" cellPadding="3" align="center">
    <tr align="center">
        <td align="center">
            <div id="divLiberacaoFinanceira" class="divGraf animated fadeInDown">
                <span class="tituloCaixa">Gestão de Emendas</span>
                <?php
                $params = [];
                $params['texto'] = 'Painel de Acompanhamento';
                $params['tipo'] = 'acompanhamento';
                $params['url'] = 'spoemendas.php?modulo=principal/emendas/painelacompanhamento/painelacompanhamento&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = '<spam class="vermelho"> Emendas Parlamentares </spam>';
                $params['tipo'] = 'listar';
                $params['url'] = 'spoemendas.php?modulo=principal/emendas/listaremendas&acao=A';
                montaBotaoInicio($params);

                montaBotaoInicio([
                    'texto' => 'Alterar dados da Emenda',
                    'tipo'  => 'processo',
                    'url'   => 'spoemendas.php?modulo=principal/alteracaoemendas/listaremendas&acao=A'
                ]);

                $params = [];
                $params['texto'] = 'Solicitações de informação / alteração';
                $params['tipo'] = 'questionario';
                $params['url'] = 'spoemendas.php?modulo=principal/solicitacoes/listarsolicitacoes&acao=A';
                montaBotaoInicio($params);
                ?>
                <span class="subTituloCaixa">Relatórios</span>
                <?php
                $params = [];
                $params['texto'] = 'Extrato Dinâmico';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'spoemendas.php?modulo=relatorio/extratodinamico&acao=A';
                montaBotaoInicio($params);
                $params = [];
                $params['texto'] = 'Carga Tesouro Gerencial';
                $params['tipo'] = 'relatorio';
                $params['url'] = 'spoemendas.php?modulo=relatorio/RelatorioCargaTesouroGerencial&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divAcoes" class="divGraf animated fadeInDown bg-ds">
                <span class="tituloCaixa">Ações <?= $_SESSION['exercicio']; ?></span> <br><br><br>
                <?php
                $params = [];
                $params['texto'] = 'Extrato completo das Ações ' . $_SESSION['exercicio'];
                $params['tipo'] = 'snapshot';
                $params['url'] = 'spoemendas.php?modulo=principal/dadosacoes/listadadosacoes&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Espelho das Ações';
                $params['tipo'] = 'espelho';
                $params['url'] = 'spoemendas.php?modulo=principal/dadosacoes/espelho&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divManuais" class="divGraf animated fadeInDown">
                <span class="tituloCaixa">Manuais</span>
                <?php
                $params = [];
                $params['texto'] = 'Manual';
                $params['tipo'] = 'pdf';
                $params['url'] = "/seguranca/downloadFile.php?download=S&esquema=public&arqid=19882801";
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Fluxo das Emendas Parlamentares';
                $params['tipo'] = 'processo';
                $params['toggle'] = 'modal';
                $params['target'] = '#fluxo';
                $params['url'] = '';
                montaBotaoInicio($params);
                ?>
            </div>
            <div id="divTabelaApoio" class="divGraf animated fadeInDown">
                <span class="tituloCaixa">Parâmetros SPO</span> <br><br><br>
                <?php
                $params = [];
                $params['texto'] = 'Iniciativas';
                $params['tipo'] = 'cadastrar';
                $params['url'] = '';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Carga dos Limites e Empenhos';
                $params['tipo'] = 'upload';
                $params['url'] = 'spoemendas.php?modulo=principal/cargas/limites/carga&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Carga do Financeiro';
                $params['tipo'] = 'upload';
                $params['url'] = 'spoemendas.php?modulo=principal/cargas/financeiro/carga&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Carga das NLs';
                $params['tipo'] = 'upload';
                $params['url'] = 'spoemendas.php?modulo=principal/cargas/nl/carga&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Carga das NEs';
                $params['tipo'] = 'upload';
                $params['url'] = 'spoemendas.php?modulo=principal/cargas/ne/carga&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Período Preenchimento Emenda';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spoemendas.php?modulo=principal/periodoreferencia/configAcessoEmendas&acao=A&aba=Periodo';
                montaBotaoInicio($params);

                ?>
            </div>

            <div id="divFinanceiro" class="divGraf animated fadeInDown" style="background-color: #3F48CC;">
                <span class="tituloCaixa">Financeiro</span>
                <?php
                $params = [];
                $params['texto'] = 'Solicitação de Financeiro';
                $params['tipo'] = 'cadastrar';
                $params['url'] = 'spoemendas.php?modulo=inicioDashboard&acao=C';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Exportar Solicitação';
                $params['tipo'] = 'espelho'; //
                $params['url'] = 'spoemendas.php?modulo=principal/financeiro/exportar_solicitacao&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Calendário Financeiro';
                $params['tipo'] = 'calendario';
                $params['url'] = 'spoemendas.php?modulo=principal/financeiro/calendario_financeiro&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Carga Tesouro Gerencial';
                $params['tipo'] = 'upload';
                $params['url'] = 'spoemendas.php?modulo=principal/cargas/financeiro/cargaTesouroGerencial&acao=A';
                montaBotaoInicio($params);

                $params = [];
                $params['texto'] = 'Carga SEGOV';
                $params['tipo'] = 'upload';
                $params['url'] = 'spoemendas.php?modulo=principal/cargas/segov/carga&acao=A';
                montaBotaoInicio($params);
                ?>
            </div>

            <br style="clear: both"/>
            <div id="divComunicados" class="divGraf animated fadeInDown">
                <span class="tituloCaixa">Comunicados</span>
                <?php
                montaComunicados();
                ?>
            </div>
        </td>
    </tr>
</table>