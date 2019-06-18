<div class="panel-body">
    <div class="col-md-11">
        <div id="conteudo_sintese">
            <?php if ($saldoNaoUtilizado && $saldoNaoUtilizado > 0) { ?>
                <div class="alert alert-warning text-center">
                    <strong>Atenção!</strong> Há saldo disponível. Caso opte por não utilizar o saldo completamente, o mesmo poderá ser distribuído para outra rede.
                </div>
            <?php } ?>
            <?php
            if (($itrid == 1) || ($itrid == 3)) {
                $tituloSintese = "Síntese da adesão ao Programa Escola Acessível - Estado {$modelInstrumentoUnidade->inudescricao}";
            } else {
                $tituloSintese = "Síntese da adesão ao Programa Escola Acessível - Município {$modelInstrumentoUnidade->inudescricao} - {$modelInstrumentoUnidade->estuf}";
            }
            ?>
            <center>
                <h3><?php echo $tituloSintese; ?></h3>
            </center>
            <hr class="divider">
            <center>
                <h3>Escolas Selecionadas</h3>
            </center>

            <?php
                $objEscolasEscolaAcessivel = new Par3_Model_EscolasEscolaAcessivel();
                $arrayParams['inuid'] = $inuid;
                $arrayParams['adpid'] = $adpid;
                $arrayParams['adpano_ciclo'] = $_SESSION['exercicio'];
                
                if (!empty($adpid) && !empty($inuid)) {
                    $objEscolasEscolaAcessivel->getListaEscolasEscolhidas($arrayParams);
                    $escolasSelecionadas = $objEscolasEscolaAcessivel->getTotalEscolasEscolhidas($arrayParams);
                } 
            ?>
            
            <hr class="divider">
            
            <?php
            if (count($arrRestricoes) > 0) {
            ?>
                <!--<hr class="divider">-->
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
                    ($esdid == WF_ESDID_EMCADASTRAMENTO_ESCOLAACESSIVEL) &&
                    ($result['adpresposta'] != 'N') &&
                    ($mostrar) &&
                    count($arrRestricoes) == 0 &&
                    (!$bloqueiaGeral) && ($escolasSelecionadas > 0)
                ) {
            ?>
                <button type="button" onclick='javascript:confirmaAdesaoEscolaAcessivel( "E", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=E&inuid=<?php echo $_GET['inuid'] ?>&adpid=<?php echo $adpid ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")'
                        class="btn btn-primary"><i class="fa fa-thumbs-up"></i> Enviar adesão para as escolas
                </button>
            <?php } else { ?>
                
            <?php } ?>
            <button type="button" onclick='javascript:gerarPDFSinteseEscolaAcessivel()' class="btn btn-warning "><i class="fa fa-print"></i> Imprimir</button>
            <form method="post" name="formulario-pdf-sintese" id="formulario-pdf-sintese" class="form form-horizontal">
                <input type="hidden" id="requisicao" name="requisicao" value="pdf_sintese"/>
                <input type="hidden" id="conteudo_pdf" name="conteudo_pdf" value=""/>
            </form>
        </center>
    </div>
    <div class="col-md-1">
        <?php if ( (!empty($docidInserido) && ($escolasSelecionadas > 0) ) &&
                (in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
                in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis) ||
                in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis) ||
                in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis)) 
                )
            { 
                wf_desenhaBarraNavegacao($docidInserido, array('inuid' => $inuid)); 
            }
        ?>
    </div>
</div>