<div class="panel-body">
    <?php $objOrientacao->getOrientacaoByID(ORIENTACOES_ESCOLAACESSIVEL_ABA_ESCOLAS, "Escolas"); ?>
    <hr class="divider">
    <div id="div_escolas" style="<?php echo $displayescolas ?>">
        <input type="hidden" value="<?php echo $esdid ?>" id="esdid_atual" name="esdid_atual"/>
        <div class="col-md-1 input-lg text-right">
            <a onclick="popupEscolaAcessivel(<?php echo $inuid ?>, <?php echo $adpid ?>, 'true', <?php echo $prgid ?>)" title="Visualizar escolas aptas" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-list primary"></span>
            </a>
        </div>
        <div class="col-md-11 input-lg">
            Lista de escolas aptas
        </div>
        
        <div class="col-md-1 input-lg text-right">
            <a onclick="popupEscolaAcessivel(<?php echo $inuid ?>, <?php echo $adpid ?>, 'false', <?php echo $prgid ?>)" title="Visualizar Escolas não aptas" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-list danger"></span>
            </a>
        </div>
        <div class="col-md-11 input-lg">
            Lista de escolas não aptas
        </div>
        
        <?php
        if (
            in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis) ||
            in_array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR, $perfis) ||
            ($esdid == WF_ESDID_ENVIADOPARAOMEC_ESCOLAACESSIVEL)
            &&
            (
                (
                (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis))
                ) ||
                in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)
            )
            && !( $esdid == WF_ESDID_TERMONAOACEITO_ESCOLAACESSIVEL  ||
                (strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$adpano_ciclo] . ' 23:59:00') < strtotime(date('Y-m-d G:i:s')))
            )
        ) {
            ?>
        
            <?php
        }
        ?>
    </div>

</div>