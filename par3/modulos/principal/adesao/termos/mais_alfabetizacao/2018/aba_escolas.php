<div class="panel-body">
    <?php $objOrientacao->getOrientacaoByID(ORIENTACOES_MAISALFABETIZACAO_ABA_ESCOLAS, "Escolas"); ?>
    <hr class="divider">
    <div id="div_escolas" style="<?php echo $displayescolas ?>">
        <input type="hidden" value="<?php echo $esdid ?>" id="esdid_atual" name="esdid_atual"/>

        <div class="col-md-1 input-lg text-right">
            <?php if ($totalEscolasGrupo1 < 1) { ?>
                <span alt="Nenhum escola foi vinculada ao GRUPO 1" title="Nenhuma escola foi vinculada ao grupo 1" aria-hidden="true"
                      class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
                <?php
            }
            ?>
            <a onclick="popupEscolasAlfabetizacao('<?php echo $inuid ?>', <?php echo $adpid ?>, '10','<?php echo $prgid ?>')" title="Selecionar Escolas do Grupo 01" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>
            </a>
        </div>

        <div class="col-md-11 input-lg">
            Selecionar Escolas do Grupo 01 <span class="badge"><?= ($totalEscolasGrupo1 > 0 ? "$totalEscolasGrupo1 selecionados" : '') ?></span>
        </div>

        <hr>
        <!--        grupo 2 -->

        <div class="col-md-1 input-lg text-right">
            <?php if ($totalEscolasGrupo2 < 1) { ?>
                <span alt="Nenhum escola foi vinculada ao GRUPO 1" title="Nenhuma escola foi vinculada ao grupo 1" aria-hidden="true"
                      class="glyphicon glyphicon-exclamation-sign danger fa-lg"></span>
                <?php
            }
            ?>
            <a onclick="popupEscolasAlfabetizacao('<?php echo $inuid ?>', <?php echo $adpid ?>, '5','<?php echo $prgid ?>')" title="Selecionar Escolas do Grupo 02" href="#">
                <span aria-hidden="true" class="glyphicon glyphicon-plus-sign success fa-lg"></span>
            </a>
        </div>

        <div class="col-md-11 input-lg">
            Selecionar Escolas do Grupo 02 <span class="badge"><?= ($totalEscolasGrupo2 > 0 ? " $totalEscolasGrupo2 selecionados" : '') ?></span>
        </div>

        <hr>

        <div class="col-md-1 input-lg text-right">
            <a onclick="popupEscolasAlfabetizacao('<?php echo $inuid ?>', <?php echo $adpid ?>, 'N','<?php echo $prgid ?>')" title="Visualizar escolas" href="#">
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
            ($esdid == WF_ESDID_ENVIADOPARAOMEC_MAISALFABETIZACAO)
            &&
            (
                (
                (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis))
                ) ||
                in_array(Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO, $perfis)
            )
            && !( $esdid == WF_ESDID_TERMONAOACEITO_MAISALFABETIZACAO  ||
                (strtotime($_SESSION['par3']['mais_escolas']['vigencia'][$adpano_ciclo] . ' 23:59:00') < strtotime(date('Y-m-d G:i:s')))
            )
        ) {
            ?>
            <div class="col-md-1 input-lg text-right">

                <a onclick="insereExcecaoEscolasAlfabetizacao('<?php echo $inuid ?>','<?php echo $adpid ?>','<?php echo $prgid ?>')"
                   title="Solicitar Inclusão de Escolas" href="#">
                    <h2 aria-hidden="true" class="glyphicon glyphicon-envelope"></h2>
                </a>
            </div>

            <div class="col-md-3 input-lg">
                Solicitar Inclusão de Escolas
                <?php
                if (in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL, $perfis) ||
                    in_array(Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL, $perfis)
                ) {
                    ?>
                    <label style="color:red; font-size: 12px;">
                        (Prazo Final: <?= formata_data($_SESSION['par3']['mais_escolas']['vigencia']['2018']) ?> 23:59)
                    </label>
                    <?php
                }
                ?>
            </div>
            <?php
        }
        ?>
    </div>

</div>