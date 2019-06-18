<div class="panel-body">     
    <div style="height: 300px;overflow-y: scroll" id="termo">
        <?php if (empty($aceite) && !isset($_SESSION[$_POST['pppid']]['termoaceito'])): ?>
            <script>
                swal({
                    title: "Bem-Vindo ao <?php echo $programa['prgdsc'];?>!",
                    text: '<?php echo str_replace(array("\n", "\r"), '', $proadesaoDados['pfamsgboasvindas']); ?>',
                    html: true
                });
            </script>
        <?php endif; ?>

        <?php
        if ($aceite['adprrdposta'] == "S") {
            echo $aceite['padtermopreenchido'];
        } else {
            if (!$termoincompleto) {
                echo $termoPreenchido;
            } else {
                echo "<div class='alert alert-danger'>É necessário o cadastramento do Dirigente Municipal de Educação na aba Dados da Unidade no PAR para gerar o Termo de Adesão.</div>";
            }
        }
        ?>
    </div>
    <?php
    if (
        (
            (empty($aceite['adpdataresposta'])) ||
            ($aceite['adpresposta'] == 'C')
        ) &&
        !$termoincompleto
    ) { ?>
        <div style="text-align: center;margin-top: 20px;">
            <?php if (($mostrar) && (!$vencido)) { ?>
                <b>Concorda com o termo de adesão ?</b><br><br>
            <?php } ?>            
            <?php if (($mostrar) && (!$vencido)) { ?>                
                <?php if ($escolasSelecionadasInuid > 0) { ?>
                    <button onclick='javascript:confirmaAdesaoEscolaAcessivel( "S", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")' class="btn btn-primary">Sim</button>
                    <button onclick='javascript:confirmaAdesaoEscolaAcessivel( "N", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=N&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")' class="btn btn-danger">Não</button>
                <?php } ?>
            <?php } ?>
            <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
        </div>

    <?php } elseif ($aceite['adpresposta'] == "S" && !$termoincompleto) { ?>
        <div style="margin-top: 5px;" class="text-right">            
            <label class="label label-primary">
                <?php
                $nome = $objUsuario->getNome($aceite['usucpf']);

                $exibeComeco = '';
                if ($nome != '') {
                    $exibeComeco = $nome . ' CPF: ';
                }
                if (strftime("%H:%M:%S", strtotime($aceite['adpdataresposta'])) != '00:00:00') { ?>
                    Assinado por <?php
                    echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data_hora($aceite['adpdataresposta']); ?>.
                    <?php
                } else {
                    ?>
                    Assinado por <?php
                    echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
                    <?php
                } ?>
            </label>
        </div>        
        <div style="text-align: center;margin-top: 20px;">
            <a href="/par3/par3.php?modulo=principal/adesao/termo&acao=A&itrid=<?php echo $itrid ?>&requisicao=pdf_termo&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>" target="_blank">
                <button class="btn btn-warning imprimir"><i class="fa fa-print"></i> Imprimir</button>
            </a>
            <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
        </div>
    <?php } else if (!$termoincompleto) {
        if (($result['adpresposta'] == 'N') && ($esdid == WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL)) {
            ?>
            <div style="margin-top: 5px;" class="text-right">
                <label class="label label-danger">
                    <?php
                    $nome = $objUsuario->getNome($aceite['usucpf']);

                    $exibeComeco = '';
                    if ($nome != '') {
                        $exibeComeco = $nome . ' CPF: ';
                    }
                    if (strftime("%H:%M:%S", strtotime($aceite['adpdataresposta'])) != '00:00:00') { ?>
                        Termo não aceito por <?php
                        echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data_hora($aceite['adpdataresposta']); ?>.
                        <?php
                    } else {
                        ?>
                        Termo não aceito por <?php
                        echo $exibeComeco . formatar_cpf($aceite['usucpf']); ?> em <?php echo formata_data($aceite['adpdataresposta']); ?>.
                        <?php
                    } ?>
                </label>
            </div>
            <div style="text-align: center;margin-top: 20px;">
                <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>

                <button onclick='javascript:confirmaAdesaoEscolaAcessivel( "C", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=C&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")'
                        class="btn btn-danger">Cancelar Resposta
                </button>
            </div>
            <?php
        } ?>
    <?php } ?>
</div>
