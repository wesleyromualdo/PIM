<script>
function downloadResolucao(arqid){
    window.location = 'par3.php?modulo=principal/adesao/termo&acao=A&prgid=<?php echo $_GET['prgid'] ?>&inuid=<?php echo $_GET['inuid'] ?>&arquivo_resolucao='+arqid;
}
function confirmaAdesao(decisao, link) {
    if (decisao == 'S') {
        location.href = link;
    }
}
</script>
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
            echo $termoPreenchido;
        }
        ?>
    </div>
    <?php
    if(empty($aceite['adpdataresposta'])){ ?>
        <div style="text-align: center;margin-top: 20px;">
            <?php if (($mostrar) && (!$vencido)) { ?>
                <b>Concorda com o termo de adesão ?</b><br><br>
                <button onclick='confirmaAdesao( "S", "/par3/par3.php?modulo=principal/adesao/termo&acao=A&aceite=S&inuid=<?php echo $_GET['inuid'] ?>&prgid=<?php echo $_GET['prgid'] ?>&adpano_ciclo=<?php echo $adpano_ciclo; ?>")'
                        class="btn btn-primary">Confirmar
                </button>
            <?php } ?>
            <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
        </div>
    <?php 
    }elseif($aceite['adpresposta'] == "S") { ?>
        <div style="margin-top: 5px;text-align: right;" class="text-right">
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
            <?php if(!empty($arquivoResolucao)) : ?>
                <button type="button" class="btn btn-success" onclick="downloadResolucao(<?php echo $arquivoResolucao; ?>);"><i class="fa fa-upload"></i> Resolução</button>
            <?php endif; ?>
            <a onclick="imprimir('pdf_termo')" target="_blank">
                <button class="btn btn-warning imprimir"><i class="fa fa-print"></i> Imprimir</button>
            </a>
            <a href="/par3/par3.php?modulo=principal/adesao/feiraoProgramas&acao=A&inuid=<?php echo $_GET['inuid'] ?>" class="btn btn-primary">Sair</a>
        </div>
    <?php } ?>
</div>