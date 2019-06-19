 <div class="panel panel-default" id="panel-escola-<?php echo $esc->escid;?>">
        <div class="panel-heading" align="center"><b><?php echo $esc->escnome;?></b>
        </div>
        <div class="panel-body">
            <div class="row">
                <div class="col-lg-6">
                    <div class="row" id="esccodinep-div">
                        <label class="col-lg-6 control-label text-right">INEP: </label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="esccodinep"><?php echo $esc->esccodinep; ?></p></div>
                    </div>

                    <div class="row" id="esc_situacao_funcionamento-div">
                        <label class="col-lg-6 control-label text-right">Situação do Funcionamento: </label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="esc_situacao_funcionamento"><?php echo $esc->escno_situacao_funcionamento; ?></p></div>
                    </div>

                    <div class="row" id="escno_situacao_imovel-div">
                        <label class="col-lg-6 control-label text-right">Situação do Imóvel: </label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="escno_situacao_imovel"><?php echo $esc->escno_situacao_imovel; ?></p></div>
                    </div>

                    <div class="row" id="escno_localizacao-div">
                        <label class="col-lg-6 control-label text-right">Localização/Zona da Escola:</label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="escno_localizacao"><?php echo $esc->escno_localizacao; ?></p></div>
                    </div>

                    <div class="row" id="escendereco_bairro-div">
                        <label class="col-lg-6 control-label text-right">Endereço/Bairro:</label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="escendereco_bairro"><?php echo $esc->escendereco_bairro; ?></p></div>
                    </div>
                    <div class="row" id="predominanciaatentimento-div">
                        <label class="col-lg-6 control-label text-right">Predominância de Atendimento:</label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="predominanciaatentimento">
                                <?php echo $labelEtapa; ?>
                            </p>
                        </div>
                    </div>
                </div>
                <div class="col-lg-6">
                    <div class="row" id="escqtd_salas-div">
                        <label class="col-lg-6 control-label text-right">Quantidade de Salas de Aula:</label>
                        <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left" id="escqtd_salas"><?php echo $esc->escqtd_salas; ?></p></div>
                    </div>
                    <br>
                    <div class="row" id="div-qtd-escolas">
                        <div class="row" id="ederemanejamento_infantil-total-div">
                            <label class="col-lg-6 control-label text-right">Quantidade Total de Alunos Ensino Infantil:</label>
                            <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left"><?php echo $esc->escqtd_alunos_infantil; ?></p></div>

                        </div>
                        <div class="row" id="ederemanejamento_fundamental-total-div">
                            <label class="col-lg-6 control-label text-right">Quantidade Total Alunos Ensino Fundamental:</label>
                            <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left"><?php echo $esc->escqtd_alunos_fundamental; ?></p></div>
                        </div>
                        <div class="row" id="ederemanejaparmento_fundamental-total-div">
                            <label class="col-lg-6 control-label text-right">Quantidade Total Alunos Ensino Médio:</label>
                            <div class="col-md-6 col-md-6 col-lg-6 control-label" ><p class="text-left"><?php echo $esc->escqtd_alunos_medio; ?></p></div>
                        </div>
                    </div>
                </div>
                <hr/>
<!--                <div class="row" id="ederemanejamento-div">-->
<!--                    <label class="col-md-offset-1 col-lg-offset-1 col-lg-3 control-label text-right">Quantidade Restante Alunos --><?//= $labelEtapa;?><!--:</label>-->
<!--                    <div class="col-md-3 col-md-3 col-lg-3 control-label" >--><?php //echo $esc['qtd']; ?><!--</div>-->
<!--                </div>-->
            </div>
        </div>
</div>
