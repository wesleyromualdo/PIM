<div id="formularioFiltros">
    <form class="form-horizontal" name="formEdicaoTerritorio" role="form" method="POST">
        <input name="formulario" value="1" type="hidden"/>
        <input name="infterid" value="<?= ($_GET['infterid']) ? $_GET['infterid'] : '' ?>" type="hidden"/>
        <input name="conid" value="<?= ($arrDados['conid']) ? $arrDados['conid'] : '' ?>" type="hidden"/>
        <input name="torid" value="<?= ($arrDados['torid']) ? $arrDados['torid'] : '' ?>" type="hidden"/>
        <form class="form-horizontal">
            <fieldset>
                <br>
                <br>
                <!-- Form Name -->
                <!--                <legend>Informações Território</legend>-->
                <!-- Text input-->
                <div class="form-group">
                    <label class="col-md-2 control-label" for="textinput">Nome da Organização:</label>
                    <div class="col-md-10">
                        <input id="nomeOrganizacao" name="infternomeorganizacao"
                               value="<?= $arrDados['infternomeorganizacao'] ?>" onkeypress="divcontatoorganizacao()"
                               type="text" placeholder="" class="form-control input-md" required="required">
                    </div>
                </div>
                <!-- Select Basic -->
                <div class="form-group">
                    <label class="col-md-2 control-label" for="selectbasic">Tipo da Organização:</label>
                    <div class="col-md-2">
                        <select id="tipoOrganizacao" name="orgid" onchange="divcontatoorganizacao();verificaCorTipoOrganizacaoCadastrada();"
                                class="form-control">
                            <option value="0">Selecione</option>
                            <?php
                            if (!empty($rowOrgaos)) {
                                foreach ($rowOrgaos as $chave => $org) { ?>
                                    <option <?= ($org['orgid'] == $arrDados['orgid']) ? 'selected' : '' ?>
                                            value="<?= $org['orgid'] ?>"><?= $org['orgdsc'] ?></option>
                                    <?php
                                }
                            }
                            ?>
                        </select>
                    </div>
                    <label class="col-md-2 control-label" for="selectbasic">Status:</label>
                    <div class="col-md-2">
                        <select id="status" name="infterstatus" onchange="divcontatoorganizacao()" class="form-control">
                            <option <?= ('A' == $arrDados['infterstatus']) ? 'selected' : '' ?> value="A">Ativo</option>
                            <option <?= ('I' == $arrDados['infterstatus']) ? 'selected' : '' ?> value="I">Inativo
                            </option>
                        </select>
                    </div>
                    <label class="col-md-2 control-label" for="selectbasic">Cor:</label>
                    <div class="col-md-2">
                        <input type="text" class="form-control" id="cor" name="inftercor">
                        <span class="help-block" id="corCadastrada" style="display: none;color: red; font-weight: ">cor já cadastrada</span>
                    </div>
                </div>
                <div id="divConatatoOrganizacao"   <?=$_GET['infterid'] == "" ? "style='display: none'" : "" ?> >
                    <hr style="border: solid 1px #CCC;">
                    <legend>Contato da Organização</legend>

                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="cpf">CPF:</label>
                        <div class="col-md-2">
                            <input id="cpf" name="contcpf" type="text" value="<?=$arrDados['contcpf']?>" placeholder="000.000.000-00"
                                   class="form-control input-md">
                        </div>
                        <label class="col-md-2 control-label" for="nome">Nome:</label>
                        <div class="col-md-6">
                            <input id="nome" name="connome" type="text"  value="<?=$arrDados['connome']?>" placeholder="" class="form-control input-md">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label"  for="cargofuncao">Cargo/Função:</label>
                        <div class="col-md-10">
                            <input id="cargofuncao" name="concargofuncao" value="<?=$arrDados['concargofuncao']?>"   type="text" placeholder=""
                                   class="form-control input-md">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="email">Email:</label>
                        <div class="col-md-10">
                            <input id="email" name="conemail" value="<?=$arrDados['conemail']?>"  type="email" placeholder="" class="form-control input-md">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="telefone1">Telefone 1:</label>
                        <div class="col-md-2">
                            <input id="telefone1" name="contelefone1" value="<?=$arrDados['contelefone1']?>"  type="text" placeholder="(00) 0000-00000"
                                   class="form-control input-md">
                        </div>
                        <label class="col-md-2 control-label" for="telefone1">Telefone 2:</label>
                        <div class="col-md-2">
                            <input id="telefone2" name="contelefone2" value="<?=$arrDados['contelefone2']?>"  type="text" placeholder="(00) 0000-00000"
                                   class="form-control input-md">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="cep">CEP:</label>
                        <div class="col-md-2">
                            <input id="cep" name="concep" type="text" placeholder="00000-000" value="<?=$arrDados['concep']?>"
                                   class="form-control input-md">
                        </div>
                        <label class="col-md-2 control-label" for="logradouro">Logradouro:</label>
                        <div class="col-md-6">
                            <input id="logradouro" name="conlogradouro" type="text" placeholder=""
                                   class="form-control input-md " value="<?=$arrDados['conlogradouro']?>"  readonly="readonly">
                        </div>
                    </div>
                    <!-- Password input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="complemento">Complemento:</label>
                        <div class="col-md-10">
                            <input id="complemento" name="concomplemento" value="<?=$arrDados['concomplemento']?>"  placeholder="" class="form-control input-md">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="bairro">Bairro:</label>
                        <div class="col-md-2">
                            <input id="bairro" name="conbairro" value="<?=$arrDados['conbairro']?>"  type="text" placeholder="" class="form-control input-md "
                                   readonly="readonly">
                        </div>
                        <label class="col-md-2 control-label" for="cidade">Cidade:</label>
                        <div class="col-md-3">
                            <input id="cidade" name="concidade" value="<?=$arrDados['concidade']?>"  type="text" placeholder="" class="form-control input-md "
                                   readonly="readonly">
                        </div>
                        <label class="col-md-2 control-label" for="estado">UF:</label>
                        <div class="col-md-1">
                            <input id="estado" name="conuf" value="<?=$arrDados['conuf']?>"  type="text" placeholder="" class="form-control input-md "
                                   readonly="readonly">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="site">Site da Organização:</label>
                        <div class="col-md-10">
                            <input id="site" name="consite" value="<?=$arrDados['consite']?>"  type="text" placeholder="" class="form-control input-md">
                        </div>
                    </div>
                </div>
                <div id="divSecretaria" <?=$_GET['infterid'] == "" ? "style='display: none'" : "" ?>>
                    <hr style="border: solid 1px #CCC;">
                    <legend>Órgão de Educação de Referência</legend>
                    <!-- Select Basic -->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="seR">Órgão de Educação:</label>
                        <div class="col-md-4">
                            <select id="secretaria" name="seceducacaoreferencial" class="form-control "
                                    onchange="buscaDadossecretaria()">
                                <option value="0">Selecione</option>
                                <?php foreach ($entidades as $item): ?>
                                    <option value="<?= $item['entid'] ?>" <?= ($item['entid'] == $arrDados['entid']) ? 'selected' : '' ?>><?= $item['entnome'] ?></option>
                                <?php endforeach; ?>
                            </select>
                        </div>
                        <label class="col-md-2 control-label" for="tipoOrgao">Tipo de Órgão:</label>
                        <div class="col-md-2">
                            <textarea id="tipoOrgaoSecretaria" name="tipoOrgaoSecretaria" class="form-control input-md" data-ls-module="charCounter" maxlength="500" ><?=$arrDados['tordesc']?></textarea>
<!--                        <input id="tipoOrgaoSecretaria" name="tipoOrgaoSecretaria" type="textarea" 
                                   class="form-control input-md"  required=""> -->
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="cpf">CPF:</label>
                        <div class="col-md-2">
                            <input id="cpfSecretaria" name="cpfSecretaria" type="text" placeholder="000.000.000-00"
                                   class="form-control input-md" readonly="readonly" required="">
                        </div>
                        <label class="col-md-2 control-label" for="nome">Nome:</label>
                        <div class="col-md-6">
                            <input id="nomeSecretaria" name="nomeSecretaria" type="text" placeholder="" 
                                   class="form-control input-md" readonly="readonly">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="email">E-mail:</label>
                        <div class="col-md-10">
                            <input id="emailSecretaria" name="emailSecretaria" type="text" placeholder=""
                                   class="form-control input-md" readonly="readonly">
                        </div>
                    </div>
                    <!-- Text input-->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="telefone1">Telefone 1:</label>
                        <div class="col-md-2">
                            <input id="telefone1Secretaria" name="telefone1Secretaria" type="text" placeholder=""
                                   class="form-control input-md" readonly="readonly">
                        </div>
                        <label class="col-md-2 control-label" for="telefone1">Telefone 2:</label>
                        <div class="col-md-2">
                            <input id="telefone2Secretaria" name="telefone2Secretaria" type="text" placeholder=""
                                   class="form-control input-md" readonly="readonly">
                        </div>
                    </div>
                </div>
                <hr style="border: solid 1px #CCC;">
                <div id="divDocumentos" <?=$_GET['infterid'] == "" ? "style='display: none'" : "" ?>>
                    <legend>Documentos</legend>
                    <!-- Select Basic -->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="Instrumento Jurídico">Instrumento Jurídico</label>
                        <div class="col-md-6">
                            <select id="instrumentoJuridico" name="instrumentoJuridico" class="form-control chosen-select">
                                <option value="0">Selecione</option>
                                <option value="1">Contrato de Consórcio Público</option>
                                <option value="2">Convênio Intermunicipal</option>
                                <option value="3">Termo, Acordo ou Protocolo de Colaboração Interfederativa</option>
                                <option value="4">Estatuto e Ata de Criação de Organização Não Governamental</option>
                                <option value="5">Plano de Ação de Território Etnoeducacional</option>
                                <option value="6">Portarias de Nomeação de Colegiado, Grupo de Trabalho, Comissão, Conselho ou Símile</option>
                                <option value="7">Outros</option>
                            </select>
                        </div>
                    </div>
                    <!-- File Button -->
                    <div class="form-group">
                        <label class="col-md-2 control-label" for="arquivo">Arquivo</label>
                        <div class="col-md-4">
                            <input id="arquivo" name="arquivo" class="input-file" type="file">
                        </div>
                        <div class="col-md-2">
                            <button id="addDocumento" name="addDocumento" type="button" class="btn btn-primary btn-block">Adicionar</button>
                        </div>
                    </div>
                    <div style="width:66%;">
                        <table class="table table-striped table-hover">
                            <thead>
                            <tr>
                                <th>Download</th>
                                <th class="pull-left">Instrumento Jurídico</th>
                            </tr>
                            </thead>
                            <tbody>

                            <?php
                            if(!empty($arrDocumentos)){
                                foreach ($arrDocumentos as $documento):
                                    $instrumento = instrumentoJuridico($documento['docterinstrumentojuridico']);
                                    ?>
                                    <tr>
                                        <th scope="row">
                                            <button type="button" id="btnArquivo" class="btn btn-success "
                                                    style="cursor:pointer;"
                                                    onclick="window.location='sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&infterid=1<?=$_GET['infterid'] ?>&requisicao=downloadAnexo&amp;arqid=<?= $documento['arqid'] ?>'" >
                                                <span class="glyphicon glyphicon-file"></span>
                                            </button>
                                        </th>
                                        <td><?=$instrumento?></td>
                                    </tr>
                                <?php endforeach;
                            }else{
                                ?>
                                <tr>
                                    <td scope="row" colspan="2">
                                        <div class="alert alert-info center" style="text-align: center">
                                            Nenhum dado Encontrado!
                                        </div>
                                    </td>

                                </tr>
                                <?php
                            }
                            ?>
                            </tbody>
                        </table>
                    </div>

                </div>
                <!-- Button -->
                <div class="form-group">
                    <div class="col-md-12">
                        <?php if(!$_GET['infterid']):?>

                            <button class="btn btn-primary btn-block " type="submit"  <?=$_GET['infterid'] == "" ? "disabled" : "" ?> id="salvarOrganizacao">
                                <span class="glyphicon glyphicon-ok"></span>  Salvar
                            </button>
                        <?php else:?>
                            <button class="btn btn-warning btn-block " type="button"  <?=$_GET['infterid'] == "" ? "disabled" : "" ?> id="salvarOrganizacao">
                                <span class="glyphicon glyphicon-pencil"></span> Editar
                            </button>
                        <?php endif;?>
                    </div>
                </div>
            </fieldset>
        </form>
</div>