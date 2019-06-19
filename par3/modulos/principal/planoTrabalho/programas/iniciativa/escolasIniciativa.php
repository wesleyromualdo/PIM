<table class="table">
    <?php
    if(count($arrEscolas) > 0){
    ?>
    <thead>
        <tr>
            <th>Munipio</th>
            <th>Escola</th>
            <th>Qtd Alunos</th>
            <th>Qtd Salas</th>
        </tr>
    </thead>
    <tbody>
    <?php
        foreach($arrEscolas as $escola){
        ?>
        <tr>
            <td><?php echo $escola['mundescricao']?></td>
            <td><?php echo $escola['escnome']?></td>
            <td><?php echo $escola['escqtdalunos']?></td>
            <td><?php echo $escola['escqtdsalas']?></td>
        </tr>
        <?php
        }
    }else{
        ?>
        <tr>
            <td colspan=4 >
                <label style="color: red;">
                    Não existeme escolas vinculadas.
                </label>
            </td>
        </tr>
        <?php
    }
    ?>
    </tbody>
</table>