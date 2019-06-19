<?php
global $simec;

    $iconPreenchido = '<span class="fa fa-check iconUnidade"' .
        'style="color:green !important;' .
        'background-color: transparent;"></span>';

    $pastaDadosUnidade = 'demandas/modulos/visao/';
?>
     <style>

        .obrigatorio{
            font-size: 10px;
            color:#9d1621;

        }
        
    </style>



    <div class="row wrapper border-bottom white-bg page-heading">
        <div class="col-lg-10">
            <h2>Cadastro de Obras</h2>
            <ol class="breadcrumb">
                <li class="breadcrumb-item">
                    <a href="index.html">Home</a>
                </li>
                <li class="breadcrumb-item">
                    <a>Forms</a>
                </li>
                <li class="breadcrumb-item active">
                    <strong>Basic Form</strong>
                </li>
            </ol>
        </div>
        <div class="col-lg-2">

        </div>
    </div>

    <div class="wrapper wrapper-content animated fadeInRight">
        <div class="row">

                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Obras</h5>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Secretaria</label>
                                    <div class="col-lg-10">
                                       <form method="post" name="formulario-entidade" id="formulario-entidade">
                                        
                                        <input type="hidden" name="requisicao" value="salvar">

                                        <?php
                                        $options = array('1' => 'SEE', '2' => 'asdfasd');

                                        echo $simec->select('entidsecretaria', 'Secretaria', null, $options )
                                        ?>

                                        <?= $simec->input ( 'enjdsc', 'Nome da Obra / ID', '', $arrAttr, array('label-size' => '4') ); ?>

                                    </form>

                                    </div>
                                </div>
                            </form>
                        </div>
                    </div>
                </div>











                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Obras</h5>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Secretaria</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Processo</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Convênio / Termo</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Ano</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>
                                <div class="hr-line-dashed"></div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Nome da Obra</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>                               
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Tipologia da Obra</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Tipo da Obra</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Classificação da Obra</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div> 
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Classificação da Obra</label>
                                    <div class="col-lg-10">
                                        <textarea class="form-control" placeholder="" style="margin-top: 0px; margin-bottom: 0px; height: 120px;">
                                            
                                        </textarea>
                                    </div>
                                </div>                                                                 
                            </form>
                        </div>
                    </div>
                </div>
                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Dados da Obra</h5>
>
                            </div>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Esfera</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div>

                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Programa</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Modalidade de Ensino</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div> 
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Classificação da Obra</label>
                                    <div class="col-lg-10">
                                        <textarea class="form-control" placeholder="" style="margin-top: 0px; margin-bottom: 0px; height: 120px;">
                                            
                                        </textarea>
                                    </div>
                                </div>                                                                 
                            </form>
                        </div>
                    </div>
                </div> 
                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Local da Obra</h5>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">CEP</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Logradouro</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>
                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Complemento</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div> 
                                 <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Bairro</label>
                                    <div class="col-lg-10">
                                        <input type="text" placeholder="" class="form-control">
                                    </div>
                                </div>                                                                 

                                <div class="form-group row">
                                    <label class="col-lg-2 col-form-label">Município</label>
                                    <div class="col-lg-10">
                                        <select class="form-control m-b" name="account">
                                            <option>Selecione</option>
                                            <option>option 2</option>
                                            <option>option 3</option>
                                            <option>option 4</option>
                                        </select>
                                    </div>
                                </div>

                                                             
                            </form>
                        </div>
                    </div>
                </div> 
                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Coordenadas Geográficas</h5>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                          
                                <div class="form-group row"><label class="col-sm-2 col-form-label">Latitude </label>

                                    <div class="col-sm-10">
                                        <div class="row">
                                            <div class="col-md-2"><input type="text" class="form-control"></div>
                                            <div class="col-md-2"><input type="text" class="form-control"></div>
                                            <div class="col-md-2"><input type="text" class="form-control"></div>
                                        </div>
                                    </div>
                                </div>       
                                <div class="form-group row"><label class="col-sm-2 col-form-label">Longitude </label>

                                    <div class="col-sm-10">
                                        <div class="row">
                                            <div class="col-md-2"><input type="text" class="form-control"></div>
                                            <div class="col-md-2"><input type="text" class="form-control"></div>
                                            <div class="col-md-2"><input type="text" class="form-control"></div>
                                        </div>
                                    </div>
                                </div> 
                                                             
                            </form>
                        </div>
                    </div>
                </div>  
 
                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Contatos</h5>
                            <div class="ibox-tools">
                                <a class="collapse-link">
                                    <i class="fa fa-chevron-up"></i>
                                </a>
                                <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                                    <i class="fa fa-wrench"></i>
                                </a>
                                <ul class="dropdown-menu dropdown-user">
                                    <li><a href="#" class="dropdown-item">Config option 1</a>
                                    </li>
                                    <li><a href="#" class="dropdown-item">Config option 2</a>
                                    </li>
                                </ul>
                                <a class="close-link">
                                    <i class="fa fa-times"></i>
                                </a>
                            </div>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                    <table class="table table-bordered">
                                        <thead>
                                        <tr>
                                            <th>Nome</th>
                                            <th>Email</th>
                                           <th>Telefone</th>
                                        </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td>1</td>
                                                <td>Mark</td>
                                                <td>Otto</td>
                                            </tr>
                                        </tbody>
                                    </table>
                                                             
                            </form>
                        </div>
                    </div>
                </div>  
                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Responsáveis pela Obra</h5>
                        </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                    <table class="table table-bordered">
                                        <thead>
                                        <tr>
                                            <th>Nome</th>
                                            <th>Email</th>
                                           <th>Telefone</th>
                                        </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td>1</td>
                                                <td>Mark</td>
                                                <td>Otto</td>
                                            </tr>
                                        </tbody>
                                    </table>
                                                             
                            </form>
                        </div>
                    </div>
                </div>  
                <div class="col-lg-12">
                    <div class="ibox ">
                        <div class="ibox-title">
                            <h5>Contatos da Obra</h5>
                         </div>
                        <div class="ibox-content">
                            <form>
                                <p class="obrigatorio">* Indica Campo Obrigatório.</p>
                                    <table class="table table-bordered">
                                        <thead>
                                        <tr>
                                            <th>Nome</th>
                                            <th>Email</th>
                                           <th>Telefone</th>
                                        </tr>
                                        </thead>
                                        <tbody>
                                            <tr>
                                                <td>1</td>
                                                <td>Mark</td>
                                                <td>Otto</td>
                                            </tr>
                                        </tbody>
                                    </table>

                                    <div class="col-sm-10">
                                        <div class="row">
                                            <div class="col-md-12">
                                                <div class="text-center">
                                                    <a href="" class="btn btn-primary btn-lg">Salvar</a>
                                                </div>
                                            </div>
                                         </div>
                                    </div>
                                </div>                                                              
                            </form>
                        </div>
                    </div>
                </div>                                                                                                          
            </div>
        </div>
    </div> 