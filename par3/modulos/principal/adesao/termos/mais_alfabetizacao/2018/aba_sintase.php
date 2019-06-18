<div class="panel-body">
    <div class="col-md-11">
        <div id="conteudo_sintese">

            <?php
            if (($itrid == 1) || ($itrid == 3)) {
                $tituloSintese = "Síntese da adesão ao Programa Mais Alfabetização - Estado {$modelInstrumentoUnidade->inudescricao}";
            } else {
                $tituloSintese = "Síntese da adesão ao Programa Mais Alfabetização - Município {$modelInstrumentoUnidade->inudescricao} - {$modelInstrumentoUnidade->estuf}";
            }
            ?>
            <center>
                <h3><?php echo $tituloSintese; ?></h3>
            </center>
            <hr class="divider">
            <center>
                <h3>Escolas Selecionadas Grupo 01</h3>
            </center>

            <?php
            $objEscolasMaisAlfabetizacao = new Par3_Model_EscolasMaisAlfabetizacao();
            $arrayParams['inuid'] = $inuid;
            $arrayParams['adpid'] = $adpid;
            $arrayParams['adpano_ciclo'] = '2018';

            $arrayParams['carga_horaria'] = '10';
            $objEscolasMaisAlfabetizacao->getListaEscolasEscolhidas($arrayParams);
            ?>

            <center>
                <h3>Escolas Selecionadas Grupo 02</h3>
            </center>

            <?php
            $arrayParams['carga_horaria'] = '5';
            $objEscolasMaisAlfabetizacao->getListaEscolasEscolhidas($arrayParams);
            ?>


            <hr class="divider">

            <center>
                <h3>Dados do Coordenador</h3>
            </center>
            <?php $objCoordenador->retornaListagemCoordenador($adpid); ?>

            <?php
            if (count($arrRestricoes) > 0) {
                ?>
                <hr class="divider">
                <center>
                    <h3>Restrições</h3>
                </center>
                <?php
                $objOrientacao->getTabelaRestricoes($arrRestricoes);
            }
            ?>
        </div>
        <center>
            <?php
            if (
                ($esdid == WF_ESDID_EMCADASTRAMENTO_MAISALFABETIZACAO) &&
                ($result['adpresposta'] != 'N') &&
                ($mostrar) &&
                count($arrRestricoes) == 0 &&
                (!$bloqueiaGeral)
            ) {?>
                    <button onclick='javascript:confirmaAdesaoAlfabetizacao( "E", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=E&inuid=<?php echo $_GET['inuid'] ?>&adpid=<?php echo $adpid ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")'
                            class="btn btn-primary">Enviar ao MEC
                    </button>
                    <?php
                } else {
                    ?>
                    <?php
            }
            ?>
            <button type="button" onclick='javascript:gerarPDFSinteseAlfabetizacao( )' class="btn btn-warning ">Imprimir</button>
            <form method="post" name="formulario-pdf-sintese" id="formulario-pdf-sintese" class="form form-horizontal">
                <input type="hidden" value="pdf_sintese" id="requisicao" name="requisicao"/>
                <input type="hidden" value="" id="conteudo_pdf" name="conteudo_pdf"/>
            </form>

        </center>
    </div>

    <div class="col-md-1">
        <?php if (!empty($docidInserido) &&
            (in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
                in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis))) { ?>
            <?php wf_desenhaBarraNavegacao($docidInserido, array('inuid' => $inuid)); ?>
        <?php } ?>
    </div>

</div>